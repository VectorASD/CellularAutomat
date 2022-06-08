#include <context.h>
#include <ctest.h>
#include <math.h> // fabs

CTEST(context_suite, util_1) {
    byte yeah = 1;
    for (int i = 0; i < 1000; i++) {
        int r = randint(-10, 20);
        if (r < -10 || r > 20) {
            yeah = 0;
            break;
        }
    }
    ASSERT_TRUE(yeah);
}

CTEST(context_suite, util_2) {
    byte yeah = 1;
    if (min(-10, 10) != -10) yeah = 0;
    if (min(19, 9) != 9) yeah = 0;
    if (min(-9, -19) != -19) yeah = 0;
    if (max(-10, 10) != 10) yeah = 0;
    if (max(19, 9) != 19) yeah = 0;
    if (max(-9, -19) != -9) yeah = 0;
    ASSERT_TRUE(yeah);
}

byte test_frange(float a, float b) {
    //printf("%f %f\n", a, fabs(a - b)); После создания каждого генеренного теста, проверяем, чтобы все были нули
    return fabs(a - b) >= 0.000003f;
}

CTEST(context_suite, util_3) {
    byte yeah = 1;
    if (test_frange(line_dist(10, 9, 5, 17), 9.433981)) yeah = 0;
    if (test_frange(line_dist(10, -9, -5, 17), 30.016663)) yeah = 0;
    if (test_frange(line_dist(1, 2, 3, 4), 2.828427)) yeah = 0;
    if (test_frange(line_dist(-9, 10, 17, -5), 30.016663)) yeah = 0;
    if (test_frange(line_dist(17, 10, -9, -5), 30.016663)) yeah = 0;
    ASSERT_TRUE(yeah);
}

/*
К сожалению 3.5 недели - на столько мало времени с разработкой движка, что даже тесты некогда делать было :/
Ну хоть убедились, что CI собран верно!!!
    Зачем делать тесты самому, если их можно сгенерирован?!)
    //struct Context ctx;
    //load_context(&ctx);
    printf("\n");
    for (int i = 0; i < 10; i++) {
        float a = randint(-100000, 100000) / 1000.;
        float b = randint(-100000, 100000) / 1000.;
        float c = randint(-100000, 100000) / 1000.;
        float d = randint(-100000, 100000) / 1000.;
        float e = randint(-100000, 100000) / 1000.;
        float f = randint(-100000, 100000) / 1000.;
        printf("    if (test_frange(dot_to_line_dist(%g, %g, %g, %g, %g, %g), %f)) yeah = 0;\n", a, b, c, d, e, f, dot_to_line_dist(a, b, c, d, e, f));
    }
*/

CTEST(context_suite, util_4) {
    byte yeah = 1;
    if (test_frange(dot_to_line_dist(9.784, 72.459, 34.74, 82.395, -17.389, 48.097), 36.494915)) yeah = 0;
    if (test_frange(dot_to_line_dist(92.101, 20.9, 93.91, -13.007, 76.225, -14.04), 17.715149)) yeah = 0;
    if (test_frange(dot_to_line_dist(-70.812, 63.966, 56.33, 7.272, -0.026, 74.941), 38.851685)) yeah = 0;
    if (test_frange(dot_to_line_dist(49.387, -32.473, -44.284, 90.58, 93.419, -16.546), 46.823990)) yeah = 0;
    if (test_frange(dot_to_line_dist(-62.083, -27.493, 20.077, -20.904, 35.157, 14.731), 38.694439)) yeah = 0;
    if (test_frange(dot_to_line_dist(65.89, 72.031, -85.721, -72.282, 54.425, -3.11), 46.521858)) yeah = 0;
    if (test_frange(dot_to_line_dist(75.815, 46.525, 44.88, 69.724, 60.608, 21.104), 29.622292)) yeah = 0;
    if (test_frange(dot_to_line_dist(82.773, 89.796, -14.93, 39.102, -2.933, 12.133), 29.517029)) yeah = 0;
    if (test_frange(dot_to_line_dist(-58.869, 73.543, 79.66, -3.152, -8.788, 0.167), 39.937061)) yeah = 0;
    if (test_frange(dot_to_line_dist(7.391, 29.129, 72.675, -72.533, 35.314, -65.08), 27.409935)) yeah = 0;
    ASSERT_TRUE(yeah);
}
