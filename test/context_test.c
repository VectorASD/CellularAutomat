#include <context.h>
#include <ctest.h>

CTEST(context_suite, first) {
    struct Context ctx;
    load_context(&ctx);
    printf("\nContext sizeof: %lu   ", sizeof(ctx));
    ASSERT_TRUE(sizeof(ctx) == 1992);
}
