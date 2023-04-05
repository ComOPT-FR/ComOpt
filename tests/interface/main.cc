#include<memory>
#include<interface/base_interface.h>
#include<interface/application.h>

int main(int argc, char** argv) {
    std::unique_ptr<interface::Application> app(new interface::Abaqus());
    std::unique_ptr<interface::BaseInterface<interface::Application>> 
        base(new interface::BaseInterface<interface::Application>(std::move(app)));
    base->init();
    base->run();
    base->finalize();
    return 0;    
}