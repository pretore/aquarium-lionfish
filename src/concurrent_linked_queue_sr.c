#include <stdlib.h>
#include <seagrass.h>
#include <triggerfish.h>
#include <lionfish.h>

bool lionfish_concurrent_linked_queue_sr_init(
        struct lionfish_concurrent_linked_queue_sr *const object,
        const uintmax_t concurrency) {
    if (!object) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!concurrency) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_CONCURRENCY_IS_ZERO;
        return false;
    }
    *object = (struct lionfish_concurrent_linked_queue_sr) {0};
    const bool result = octopus_concurrent_linked_queue_init(
            &object->queue,
            sizeof(struct triggerfish_strong *),
            concurrency);
    if (!result) {
        seagrass_required_true(
                OCTOPUS_CONCURRENT_LINKED_QUEUE_ERROR_MEMORY_ALLOCATION_FAILED
                == octopus_error);
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    return result;
}

static void on_destroy(void *const object) {
    seagrass_required_true(triggerfish_strong_release(object));
}

bool lionfish_concurrent_linked_queue_sr_invalidate(
        struct lionfish_concurrent_linked_queue_sr *const object) {
    if (!object) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL;
        return false;
    }
    seagrass_required_true(octopus_concurrent_linked_queue_invalidate(
            &object->queue, on_destroy));
    *object = (struct lionfish_concurrent_linked_queue_sr) {0};
    return true;
}

bool lionfish_concurrent_linked_queue_sr_concurrency(
        const struct lionfish_concurrent_linked_queue_sr *const object,
        uintmax_t *const out) {
    if (!object) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        lionfish_error = LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL;
        return false;
    }
    seagrass_required_true(octopus_concurrent_linked_queue_concurrency(
            &object->queue, out));
    return true;
}

bool lionfish_concurrent_linked_queue_sr_add(
        struct lionfish_concurrent_linked_queue_sr *const object,
        struct triggerfish_strong *const item) {
    if (!object) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!item) {
        lionfish_error = LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_NULL;
        return false;
    }
    if (!triggerfish_strong_retain(item)) {
        seagrass_required_true(TRIGGERFISH_STRONG_ERROR_OBJECT_IS_INVALID
                               == triggerfish_error);
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_INVALID;
        return false;
    }
    const bool result = octopus_concurrent_linked_queue_add(&object->queue, &item);
    if (!result) {
        seagrass_required_true(
                OCTOPUS_CONCURRENT_LINKED_QUEUE_ERROR_MEMORY_ALLOCATION_FAILED
                == octopus_error);
        seagrass_required_true(triggerfish_strong_release(item));
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED;
    }
    return result;
}

bool lionfish_concurrent_linked_queue_sr_remove(
        struct lionfish_concurrent_linked_queue_sr *const object,
        struct triggerfish_strong **const out) {
    if (!object) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        lionfish_error = LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = octopus_concurrent_linked_queue_remove(
            &object->queue, (void **) out);
    if (!result) {
        seagrass_required_true(
                OCTOPUS_CONCURRENT_LINKED_QUEUE_ERROR_QUEUE_IS_EMPTY
                == octopus_error);
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY;
    }
    return result;
}

bool lionfish_concurrent_linked_queue_sr_peek(
        struct lionfish_concurrent_linked_queue_sr *const object,
        const struct triggerfish_strong **const out) {
    if (!object) {
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL;
        return false;
    }
    if (!out) {
        lionfish_error = LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL;
        return false;
    }
    const bool result = octopus_concurrent_linked_queue_peek(
            &object->queue, (void **) out);
    if (!result) {
        seagrass_required_true(
                OCTOPUS_CONCURRENT_LINKED_QUEUE_ERROR_QUEUE_IS_EMPTY
                == octopus_error);
        lionfish_error =
                LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY;
    }
    return result;
}

