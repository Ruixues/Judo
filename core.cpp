#include "core.h"
#include "parser/parser.h"
#include "builtIn/builtIn.h"
#include <queue>

void Module::Parse() {
    // 开始解析
    ReadAToken();
    EnterScope();
    while (true) {
        if (nowToken->type == token_eof) {
            break;
        }
        auto tmp = HandleToken(nowToken);
        if (!tmp) {
            std::cout << "Generate code error." << std::endl;
            return;
        }
        tmp->genCode();
    }
    ExitScope();
}

Module::Module(std::string file, Judo *core) : Builder(core->context), core(core), Type(this), filePath(file) {

}
std::unique_ptr<llvm::Module> Module::Compile () {
    this->file.open(filePath, std::ios::in);
    if (!this->file.good()) {
        return nullptr;
    }
    this->file >> std::noskipws;
    reader = std::make_unique<RxReader>(&(this->file));
    loger = std::make_unique<Log>();
    opHandler = std::make_unique<OpHandler>(this);
    module = std::make_unique<llvm::Module>(this->filePath, core->context);
    FPM = std::move(core->InitializeModuleAndPassManager(module.get()));
    BindAllBuiltIn(this);
    Parse();
    return std::move(module);
}
std::unique_ptr<AST::ExprAST> Module::HandleToken(std::shared_ptr<RToken> token) {
    switch (token->type) {
        case token_func: {
            return Parser::ParseFunction(this);
        }
        case token_str:
            return Parser::ParseIdentifierExpr(this);
        case token_extern:
            return Parser::ParseExtern(this);
        case token_var:
            return Parser::ParserVariableDefine(this, true);
        case token_import:
            return Parser::ParseImport(this);
        case token_class:
            return Parser::ParseClass(this);
    }
    return loger->ParseError("Core", "unexpected token type:" + std::to_string(token->type));
}

Judo::Judo(std::string EnterFile) {
    static bool inited = false;
    if (!inited) {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
        inited = true;
    }
    JIT = std::make_unique<RJIT>();
    mainModule = new Module(EnterFile, this);
    modules[EnterFile] = std::shared_ptr<Module>(mainModule);
    auto mainModuleLLVM = mainModule->Compile();
    if (!mainModuleLLVM) {
        return;
    }
    JIT->addModule(std::move(mainModuleLLVM));
    auto mainSymbol = JIT->findSymbol("main");
    if (mainSymbol) {
        void (*FP)() = (void (*)()) (intptr_t) cantFail(mainSymbol.getAddress());
        FP();
    } else {
        return;
    }
}

std::unique_ptr<llvm::legacy::FunctionPassManager> Judo::InitializeModuleAndPassManager(llvm::Module* module) {
    auto FPM = std::make_unique<llvm::legacy::FunctionPassManager>(module);
    module->setDataLayout(JIT->getTargetMachine().createDataLayout());
    FPM->add(llvm::createInstructionCombiningPass());
    FPM->add(llvm::createReassociatePass());
    FPM->add(llvm::createGVNPass());
    //FPM->add(llvm::createCFGSimplificationPass());
    FPM->add(llvm::createInstructionCombiningPass());
    FPM->add(llvm::createReassociatePass());
    FPM->doInitialization();
    return FPM;
}

llvm::Function *Module::getFunction(std::string Name) {
    if (auto *F = module->getFunction(Name))    //module内还没有
        return F;
    //查看是否已经定义了
    auto FI = FunctionProto.find(Name);
    if (FI != FunctionProto.end())
        return FI->second->genFunction();
    return nullptr;
}

llvm::Value *Module::CreateAlloca(llvm::Function *Function,
                                  const std::string &Name, llvm::Type *type) {
    llvm::IRBuilder<> tmp(&Function->getEntryBlock(),
                          Function->getEntryBlock().begin());
    return tmp.CreateAlloca(type, 0, Name.c_str());
}

llvm::Value *Module::GetNamedValue(const std::string &Name) {
    auto v = namedValues.find(Name);
    if (v == namedValues.end()) {
        return nullptr;
    }
    return v->second.top();
}

void Module::SetNamedValue(const std::string &Name, llvm::Value *Value) {
    if (namedValues.find(Name) == namedValues.end()) {
        namedValues[Name] = std::stack<llvm::Value *>();
    }
    namedValues[Name].push(Value);
    ScopeVariables.top().push_back(Name);
}

void Module::EraseValue(const std::string &Name) {
    auto v = namedValues.find(Name);
    if (v == namedValues.end()) {
        return;
    }
    v->second.pop();
}

void Module::EnterScope() {
    //进入新的作用域
    ScopeVariables.push(std::vector<std::string>());
}

void Module::ExitScope() {
    if (ScopeVariables.empty()) return;
    auto now = ScopeVariables.top();
    for (auto &name:now) {
        EraseValue(name);
    }
    ScopeVariables.pop();
}
bool Judo::CallFunctionFromMainModule(std::string FunctionName) {

    return true;
}
Module::~Module() {

}
bool Judo::RequireModule(std::string ModulePath) {
    // 要求加载Module
    if (modules.find(ModulePath) != modules.end()) {
        return true;    //已经加载完成了
    }
    auto module = std::make_shared<Module>(ModulePath,this);
    auto m = module->Compile();
    if (!m) return false;
    JIT->addModule(std::move(m));
    modules [ModulePath] = module;
    return true;
}