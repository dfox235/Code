#include <stdio.h>
#include "observateur.h"
#include "robot.h"

Etat etat_initale(){
    return Init;
}

Etat transition(Etat e, Alphabet a){
    switch (e)
    {
    case Init :
        switch (a)
        {
        case M :
            return Mes;
            break;

        case A :
            return Puit;
            break;

        case G :
            return Init;
            break;

        case D :
            return Init;
            break;
        
        default:
            break;
        }
        break;

    case Mes :
        switch (a)
        {
        case M :
            return Mes;
            break;

        case A :
            return Init;
            break;

        case G :
            return Init;
            break;

        case D :
            return Init;
            break;
        
        default:
            break;
        }
        break;

    case Puit :
        switch (a)
        {
        case M :
            return Puit;
            break;

        case A :
            return Puit;
            break;

        case G :
            return Puit;
            break;

        case D :
            return Puit;
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
}




// Exercice 5 TP9 : Définition d'un observateur
// Cet automate d'états fini vérifie la propriété «aprés une rotation vers la gauche ou vers la droite, une mesure est toujours effectué»
// Etat transition(Etat e, Alphabet a){
//     switch (e)
//     {
//     case Init :
//         switch (a)
//         {
//         case M :
//             return Init;
//             break;

//         case A :
//             return Init;
//             break;

//         case G :
//             return Mes;
//             break;

//         case D :
//             return Mes;
//             break;
        
//         default:
//             break;
//         }
//         break;

//     case Mes :
//         switch (a)
//         {
//         case M :
//             return Init;
//             break;

//         case A :
//             return Puit;
//             break;

//         case G :
//             return Puit;
//             break;

//         case D :
//             return Puit;
//             break;
        
//         default:
//             break;
//         }
//         break;

//     case Puit :
//         switch (a)
//         {
//         case M :
//             return Puit;
//             break;

//         case A :
//             return Puit;
//             break;

//         case G :
//             return Puit;
//             break;

//         case D :
//             return Puit;
//             break;
        
//         default:
//             break;
//         }
//         break;
    
//     default:
//         break;
//     }
// }

int est_accepteur(Etat e){
    if (e == Init || e == Mes){
        return 1;
    }
    else if ( e == Puit){
        return 0;
    }
    return 0;
}

