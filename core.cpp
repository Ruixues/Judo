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
            return ;
        }
        tmp->genCode();
    }
    ExitScope();
    module->print(llvm::errs(), nullptr);
    auto main = getFunction("main");
    if (main) {
        //那就可以开始跑main函数了
        auto H = core->JIT->addModule(std::move(module));
        core->InitializeModuleAndPassManager();
        auto mainSymbol = core->JIT->findSymbol("main");
        if (mainSymbol) {
            void (*FP)() = (void (*)()) (intptr_t) cantFail(mainSymbol.getAddress());
            FP();
        } else {
            return;
        }
        core->JIT->removeModule(H);
    }
}

Module::Module(std::string file, Judo *core) : Builder(core->context), core(core), Type(this) {
    this->file.open(file, std::ios::in);
    if (!this->file.good()) {
        std::cout << "Open Module " << file << " Error" << std::endl;
        return;
    }
    this->file >> std::noskipws;
    reader = std::make_unique<RxReader>(&(this->file));
    loger = std::make_unique<Log>();
    opHandler = std::make_unique<OpHandler>(this);
    module = std::make_unique<llvm::Module>(file, core->context);
    llvm::ConstantFP::get(core->context, llvm::APFloat(1.0));
    BindAllBuiltIn(this);
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
    InitializeModuleAndPassManager();
    mainModule = new Module(EnterFile, this);
    modules[EnterFile] = std::shared_ptr<Module>(mainModule);
    // 准备开始解析模块
    mainModule->Parse();
}

void Judo::InitializeModuleAndPassManager() {
    auto module = std::make_unique<llvm::Module>("RJIT", context);
    FPM = std::make_unique<llvm::legacy::FunctionPassManager>(module.get());
    module->setDataLayout(JIT->getTargetMachine().createDataLayout());
    FPM->add(llvm::createInstructionCombiningPass());
    FPM->add(llvm::createReassociatePass());
    FPM->add(llvm::createGVNPass());
    //FPM->add(llvm::createCFGSimplificationPass());
    FPM->add(llvm::createInstructionCombiningPass());
    FPM->add(llvm::createReassociatePass());
    FPM->doInitialization();
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
    if (!Function) {

    }
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

Module::~Module() {

}