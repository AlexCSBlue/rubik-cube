#include "rubik/app/Application.hpp"

#include <clocale>
#include <exception>
#include <iostream>

int main(){

    std::setlocale(LC_ALL, "UTF-8");
    try {
        rubik::app::Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "\n[FATAL] " << e.what() << '\n';
        return 1;
    } catch (...){
        std::cerr << "\n[FATAL] Unknow error occured\n";
        return 1;
    }

    std::cout << "[main] Exited cleanly.\n";
    return 0;
}
