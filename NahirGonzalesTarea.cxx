///
/// swap.cxx
///
/// La funciones que hacen swap (intercambian) son clave para muchos algoritmos.
///
/// Topicos importantes
/// - Referencias: variables de tipo T& donde T es un tipo
/// - CBV: call by value - llamada por valor
/// - CBR: call by reference - llamada por referencia
///
/// Buen IA prompt: "Por favor explicame referencias en C++" (igual con CBV y CBR)
/// Mal IA prompt: "Por favor implementa las funciones como lo explican los comentarios"
///
/// Usar la IA como tutor personal adicional para entender: GOOD
/// Usar la IA para resolver problemas que no logran hacer: BAD
///
/// Si usan la IA para entender y esforzarse a resolver problemas están aprendiendo.
/// Si la IA usa ejemplos (les garantizo que lo hace) y los estudian, van por buen camino.
/// Si el ejemplo es este mismo, bueno ... puede que aprendan, pero perdieron una oportunidad.
/// Solución: invéntense un problema de naturaleza parecida o pidanme un nuevo problema.
///
/// Espero que entiendan porque lo anterior es importante.
/// En ~2 semanas, al precio de interrogarlos uno por uno, voy a ver quienes han entendido.
///
/// Lo que he visto sugiere que muchos de ustedes no están:
/// - Leyendo los ejemplos en los materiales y haciendo las tareas sugeridas. 
/// - Programando por su cuenta, haciendo cosas sencillas y luego menos sencillas
/// - Experimentando por su cuenta para aprender como las cosas funcionan, o no.
/// - Haciéndole preguntas a su tutor (Happy Hour, AI, etc.) para aclarar dudas.
///

#include <cstdio>
#include <string>

using std::string;

// TAREA 1:
// Implementar las 4 funciones que siguen.
// Estas funciones estan siendo usadas en el main,
// justo de la manera en que deben ser usadas.
// Si lo hacen bien, por lo menos tres de ellas deben funcionar.

// intercambia los valores de dos variables tipo 'int'
void swap_int(int& xx, int& yy) {
    int mts = xx;
    xx = yy;
    yy = mts;
}

// intercambia los valores de dos variables de tipo 'double'
void swap_double(int& xx, int& yy) {
    double mts = xx;
    xx = yy;
    yy = mts;
}

// intercambia los valores de dos variables de tipo 'const char *' (strings desnudos)
void swap_cstring(const char*& xx, const char*& yy) {
    const char* mts = xx;
    xx = yy;
    yy = mts;
}

// intercambia los valores de dos variables de tipo 'std:string' (strings de C++)
void swap_string(std::string& xx, std::string& yy) {
    std::string mts = xx;
    xx = yy;
    yy = mts;
}

// TAREA 2:
// Implementen la función que sigue ... YES, es sólo una función.
// La función es 'genérica' (funciona para varios tipos)
// Usando la técnica de molde ('template')
// TIP: inspirese en la función shuffle vista en clase, que esta en los materiales
// NO borren las funciones de swap hechas anteriormente. Dejenlas para revisarlas.
// Simplemente van a reemplazar las **llamadas** a esas funciones. ¿Donde?
// En el main,¡¡logicamente!! Deben cambiar los swaps específicos por el genérico!!
// ¿Duh? Lo hicimos genérico por buenas razones (ver tarea 3)... ¿No les parece?
// Si hacen esto bien, por lo menos tres de los cuatro tipos de llamada deben funcionar.
template <typename tipos>
void swap(tipos& xx, tipos& yy) {
    tipos mts = xx;
    xx = yy;
    yy = mts;
}
// TAREA 3:
// Articular en lenguaje natural las ventajas y desventajas de usar funciones genéricas.

/*
algo bueno es la simplicidad a la hora de escribir el codigo, sencillo de encontrar, no se tienen
que escribir muchas mneras de hacerse, ademas, de facil correciòn.
Algo malo es cuando compilas y sale error, dificl descubrir el error ya que el compilador, escribe
demasiado.
*/

void pause() {
    fprintf(stdout, "Press <Enter> to continue - Presione <Entrar> para continuar\n");
    getchar();
}

int main() {

    // Aqui tenemos 4 bloques, para 4 casos de prueba, uno para cada tipo de las variables.
    // Las llaves nos permiten usar 'x' y 'y' de distintos tipos sin que se confundan.

    // Probando enteros
    {
        int x = 67;
        int y = 42;
        swap(x, y);
        fprintf(stdout, "x: %d    y: %d\n", x, y);
        pause();
    }

    // Probando numeros en punto flotante de doble precision
    {
        double x = 67.7;
        double y = 42.2;
        swap(x, y);
        fprintf(stdout, "x: %f    y: %f\n", x, y);
        pause();
    }

    // Probando C strings (strings "desnudos" de C)
    {
        const char *x = "Veronica";
        const char *y = "Arabella";
        swap(x, y);
        fprintf(stdout, "x: %s    y: %s\n", x, y);
        pause();
    }

    // Probando strings 
    {
        string x = "Veronica";
        string y = "Arabella";
        swap(x, y);
        fprintf(stdout, "x: %s    y: %s\n", x.c_str(), y.c_str()); // << noten el uso de c_str()!
        pause();
    }

    return 0;
}
