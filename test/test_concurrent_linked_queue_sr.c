#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <time.h>
#include <triggerfish.h>
#include <lionfish.h>

#include <test/cmocka.h>

static void check_invalidate_error_on_object_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_invalidate(NULL));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_invalidate(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object = {};
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_init_error_on_object_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_init(
            NULL, 1));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_init_error_on_concurrency_is_zero(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_init(
            (void *) 1, 0));
    assert_int_equal(
            LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_CONCURRENCY_IS_ZERO,
            lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_init(void **state) {
    srand(time(NULL));
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    const uintmax_t check = 1 + (rand() % UINT8_MAX);
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, check));
    uintmax_t out;
    assert_true(octopus_concurrent_linked_queue_concurrency(
            &object.queue, &out));
    assert_int_equal(out, check);
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_init_error_on_memory_allocation_failed(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden
            = posix_memalign_is_overridden = true;
    assert_false(lionfish_concurrent_linked_queue_sr_init(&object, 1));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden
            = posix_memalign_is_overridden = false;
    assert_int_equal(
            LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED,
            lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_concurrency_error_on_object_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_concurrency(
            NULL, (void *) 1));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_concurrency_error_on_out_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_concurrency(
            (void *) 1, NULL));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_concurrency(void **state) {
    srand(time(NULL));
    lionfish_error = LIONFISH_ERROR_NONE;
    const uintmax_t check = 1 + (rand() % UINT8_MAX);
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, check));
    size_t out;
    assert_true(lionfish_concurrent_linked_queue_sr_concurrency(&object, &out));
    assert_int_equal(out, check);
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_add_error_on_object_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_add(NULL, (void *) 1));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_add_error_on_item_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_add((void *) 1, NULL));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_add_error_on_item_is_invalid(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    const uintmax_t invalid = 0;
    struct triggerfish_strong *strong = (struct triggerfish_strong *) &invalid;
    assert_false(lionfish_concurrent_linked_queue_sr_add((void *) 1, strong));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_INVALID,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void on_destroy(void *a) {

}

static void check_add(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, 8));
    struct triggerfish_strong *item;
    assert_true(triggerfish_strong_of(malloc(1), on_destroy, &item));
    assert_true(lionfish_concurrent_linked_queue_sr_add(&object, item));
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    assert_true(triggerfish_strong_release(item));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_add_error_on_memory_allocation_failed(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, 8));
    struct triggerfish_strong *item;
    assert_true(triggerfish_strong_of(malloc(1), on_destroy, &item));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden
            = posix_memalign_is_overridden = true;
    assert_false(lionfish_concurrent_linked_queue_sr_add(&object, item));
    malloc_is_overridden = calloc_is_overridden = realloc_is_overridden
            = posix_memalign_is_overridden = false;
    assert_int_equal(
            LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED,
            lionfish_error);
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    assert_true(triggerfish_strong_release(item));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_remove_error_on_object_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_remove(NULL, (void *) 1));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_remove_error_on_out_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_remove((void *) 1, NULL));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_remove_error_on_queue_is_empty(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, 8));
    struct triggerfish_strong *out;
    assert_false(lionfish_concurrent_linked_queue_sr_remove(&object, &out));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY,
                     lionfish_error);
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_remove(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, 8));
    struct triggerfish_strong *check;
    assert_true(triggerfish_strong_of(malloc(1), on_destroy, &check));
    assert_true(lionfish_concurrent_linked_queue_sr_add(&object, check));
    assert_true(triggerfish_strong_release(check));
    struct triggerfish_strong *out;
    assert_true(lionfish_concurrent_linked_queue_sr_remove(&object, &out));
    assert_ptr_equal(out, check);
    assert_true(triggerfish_strong_release(out));
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_peek_error_on_object_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_peek(NULL, (void *) 1));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_peek_error_on_out_is_null(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    assert_false(lionfish_concurrent_linked_queue_sr_peek((void *) 1, NULL));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL,
                     lionfish_error);
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_peek_error_on_queue_is_empty(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, 8));
    const struct triggerfish_strong *out;
    assert_false(lionfish_concurrent_linked_queue_sr_peek(&object, &out));
    assert_int_equal(LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY,
                     lionfish_error);
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

static void check_peek(void **state) {
    lionfish_error = LIONFISH_ERROR_NONE;
    struct lionfish_concurrent_linked_queue_sr object;
    assert_true(lionfish_concurrent_linked_queue_sr_init(&object, 8));
    struct triggerfish_strong *check;
    assert_true(triggerfish_strong_of(malloc(1), on_destroy, &check));
    assert_true(lionfish_concurrent_linked_queue_sr_add(&object, check));
    assert_true(triggerfish_strong_release(check));
    const struct triggerfish_strong *out;
    assert_true(lionfish_concurrent_linked_queue_sr_peek(&object, &out));
    assert_ptr_equal(out, check);
    assert_true(lionfish_concurrent_linked_queue_sr_invalidate(&object));
    lionfish_error = LIONFISH_ERROR_NONE;
}

int main(int argc, char *argv[]) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(check_invalidate_error_on_object_is_null),
            cmocka_unit_test(check_invalidate),
            cmocka_unit_test(check_init_error_on_object_is_null),
            cmocka_unit_test(check_init_error_on_concurrency_is_zero),
            cmocka_unit_test(check_init),
            cmocka_unit_test(check_init_error_on_memory_allocation_failed),
            cmocka_unit_test(check_concurrency_error_on_object_is_null),
            cmocka_unit_test(check_concurrency_error_on_out_is_null),
            cmocka_unit_test(check_concurrency),
            cmocka_unit_test(check_add_error_on_object_is_null),
            cmocka_unit_test(check_add_error_on_item_is_null),
            cmocka_unit_test(check_add_error_on_item_is_invalid),
            cmocka_unit_test(check_add),
            cmocka_unit_test(check_add_error_on_memory_allocation_failed),
            cmocka_unit_test(check_remove_error_on_object_is_null),
            cmocka_unit_test(check_remove_error_on_out_is_null),
            cmocka_unit_test(check_remove_error_on_queue_is_empty),
            cmocka_unit_test(check_remove),
            cmocka_unit_test(check_peek_error_on_object_is_null),
            cmocka_unit_test(check_peek_error_on_out_is_null),
            cmocka_unit_test(check_peek_error_on_queue_is_empty),
            cmocka_unit_test(check_peek),
    };
    //cmocka_set_message_output(CM_OUTPUT_XML);
    return cmocka_run_group_tests(tests, NULL, NULL);
}
