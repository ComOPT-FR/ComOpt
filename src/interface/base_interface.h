#pragma once
#include <iostream>
#include <memory>

namespace interface{
template <typename Application> class BaseInterface {
public:
    BaseInterface(std::unique_ptr<Application> app) : _app(std::move(app)) {}
    virtual ~BaseInterface() {}
    virtual void init() { _app->init();}
    virtual void run() { _app->run();}
    virtual void finalize() { _app->finalize();}

private:
    std::unique_ptr<Application> _app;
};
}