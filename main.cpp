#include <iostream>
#include <stdio.h>
#include <intercom.hpp>
#include <getch.hpp>

int main(int argc, char *argv[])
{
    Intercom intercom;

    while (1) {
        char aaa = getch();

        if (aaa){
            intercom.parsing(aaa);
        }
    }
    return 0;
}
