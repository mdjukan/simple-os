#include "printing.hpp"

#define LEVEL_1_IMPLEMENTED 1
#define LEVEL_2_IMPLEMENTED 1
#define LEVEL_3_IMPLEMENTED 0
#define LEVEL_4_IMPLEMENTED 0


// TEST 1 (zadatak 2, niti C API i sinhrona promena konteksta)
#include "Threads_C_API_test.hpp"
// TEST 2 (zadatak 2., niti CPP API i sinhrona promena konteksta)
#include "Threads_CPP_API_test.hpp"
// TEST 7 (zadatak 2., testiranje da li se korisnicki kod izvrsava u korisnickom rezimu)
#include "System_Mode_test.hpp"

//ok 1, 2, 7
//nok 3, 4, 5, 6
void userMain() {
    printString("Unesite broj testa? [1-7]\n");
    int test = 7;

    if ((test >= 1 && test <= 2) || test == 7) {
        if (LEVEL_2_IMPLEMENTED == 0) {
            printString("Nije navedeno da je zadatak 2 implementiran\n");
            return;
        }
    }

    switch (test) {
        case 1:
            Threads_C_API_test();
            printString("TEST 1 (zadatak 2, niti C API i sinhrona promena konteksta)\n");
            break;
        case 2:
            Threads_CPP_API_test();
            printString("TEST 2 (zadatak 2., niti CPP API i sinhrona promena konteksta)\n");
            break;
        case 7:
            System_Mode_test();
            printString("Test se nije uspesno zavrsio\n");
            printString("TEST 7 (zadatak 2., testiranje da li se korisnicki kod izvrsava u korisnickom rezimu)\n");
            break;
        default:
            printString("Niste uneli odgovarajuci broj za test\n");
    }
}