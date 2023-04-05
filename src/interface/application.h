#pragma once
#include <iostream>

namespace interface
{
    class Application
    {
    public:
        Application() {}
        virtual void init() = 0;
        virtual void run() = 0;
        virtual void finalize() = 0;
        virtual ~Application() {}
    };

    class Abaqus : public Application
    {
    public:
        Abaqus() {}
        void init(){
            // TODO: Initialize Abaqus
            std::cout << "Abaqus::init()" << std::endl;
        }
        void run(){
            // TODO: Run Abaqus
            std::cout << "Abaqus::run()" << std::endl;
        }
        void finalize(){
            //TODO: Finalize Abaqus
            std::cout << "Abaqus::finalize()" << std::endl;
        }
    };
}