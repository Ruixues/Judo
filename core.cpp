#include "core.h"
#include "parser/parser.h"
#include "builtIn/builtIn.h"
#include "jit/jit.h"
void Module::Parse() {
    // 开始解析
    ReadAToken();
    while (1) {
        if (nowToken->type == token_eof) {
            break;
        }
        auto tmp = HandleToken(nowToken);
        tmp->genCode();
    }
    module->print(llvm::errs(), nullptr);
    auto main = getFunction("main");
    if (main) {
        //那就可以开始跑main函数了
        auto H = core->JIT->addModule(std::move(module));
        core->InitializeModuleAndPassManager();
        auto mainSymbol = core->JIT->findSymbol("main");
        if (mainSymbol) {
            void (*FP)() = (void (*)())(intptr_t)cantFail(mainSymbol.getAddress());
            FP ();
        } else {
            return ;
        }
        core->JIT->removeModule(H);
    }
}

Module::Module(std::string file,Judo* core) : Builder(core->context),core (core) {
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
    llvm::ConstantFP::get(core->context,llvm::APFloat(1.0));
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
            Parser::ParseExtern(this);
            return nullptr;
        case token_sign:
            if (token->IsSign("{")) {   //代码块
                return Parser::ParseCodeBlock(this);
            }
            break;
        case token_var:
            return Parser::ParserVariableDefine(this);
        default:
            return loger->ParseError("Core", "unexpected token type");
    }
}
Judo::Judo(std::string EnterFile) {
    static bool inited = false;
    if (!inited) {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
        inited = true;
    }
    mainModule = new Module(EnterFile,this);
    modules[EnterFile] = std::shared_ptr<Module>(mainModule);
    JIT = std::make_unique<RJIT>();
    InitializeModuleAndPassManager();
    // 准备开始解析模块
    mainModule->Parse();
}
void Judo::InitializeModuleAndPassManager(void) {
    auto module = std::make_unique<llvm::Module>("RJIT", context);
    module->setDataLayout(JIT->getTargetMachine().createDataLayout());
    this->FPM = std::make_unique<llvm::legacy::FunctionPassManager>(module.get());
    FPM->add(llvm::createInstructionCombiningPass());
    FPM->add(llvm::createReassociatePass());
    FPM->add(llvm::createGVNPass());
    FPM->add(llvm::createCFGSimplificationPass());
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