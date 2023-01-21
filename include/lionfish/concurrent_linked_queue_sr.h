#ifndef _LIONFISH_CONCURRENT_LINKED_QUEUE_H_
#define _LIONFISH_CONCURRENT_LINKED_QUEUE_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <octopus.h>

struct triggerfish_strong;

#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL            1
#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_CONCURRENCY_IS_ZERO       2
#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED  3
#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL               4
#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_NULL              5
#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_INVALID           6
#define LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY            7

struct lionfish_concurrent_linked_queue_sr {
    struct octopus_concurrent_linked_queue queue;
};

/**
 * @brief Initialize concurrent linked queue.
 * @param [in] object instance to be initialized.
 * @param [in] concurrency maximum number of concurrent reads or writes that
 * can occur.
 * @return On success true, otherwise false if an error has occurred.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL if object
 * is <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_CONCURRENCY_IS_ZERO if
 * concurrency is zero.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED
 * if there is insufficient memory to initialize instance.
 */
bool lionfish_concurrent_linked_queue_sr_init(
        struct lionfish_concurrent_linked_queue_sr *object,
        uintmax_t concurrency);

/**
 * @brief Invalidate concurrent linked queue.
 * <p>All the items contained within the queue will have the given <i>on
 * destroy</i> callback invoked upon itself. The actual <u>concurrent queue
 * instance is not deallocated</u> since it may have been embedded in a larger
 * structure.</p>
 * @param [in] object instance to be invalidated.
 * @param [in] on_destroy called just before the item is to be destroyed.
 * @return On success true, otherwise false if an error has occurred.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL if object
 * is <i>NULL</i>.
 */
bool lionfish_concurrent_linked_queue_sr_invalidate(
        struct lionfish_concurrent_linked_queue_sr *object);

/**
 * @brief Retrieve the concurrency limit.
 * @param [in] object instance whose concurrency limit we are to retrieve.
 * @param [out] out receive the concurrency limit.
 * @return On success true, otherwise false if an error has occurred.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL if object
 * is <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL if out is
 * <i>NULL</i>.
 */
bool lionfish_concurrent_linked_queue_sr_concurrency(
        const struct lionfish_concurrent_linked_queue_sr *object,
        uintmax_t *out);

/**
 * @brief Add item to the end of the queue.
 * @param [in] object queue instance.
 * @param [in] item to add to the end of the queue.
 * @return On success true, otherwise false if an error has occurred.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL if object
 * is <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_NULL if item is
 * <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_ITEM_IS_INVALID if the
 * strong reference of item has been invalidated.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_MEMORY_ALLOCATION_FAILED
 * if there is insufficient memory to add item.
 * @note <b>item</b> is retained and then it is inserted.
 */
bool lionfish_concurrent_linked_queue_sr_add(
        struct lionfish_concurrent_linked_queue_sr *object,
        struct triggerfish_strong *item);

/**
 * @brief Remove item from the front of the linked queue.
 * @param [in] object linked queue instance.
 * @param [out] out receive the item in the front of the linked queue.
 * @return On success true, otherwise false if an error has occurred.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL if object
 * is <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL if out is
 * <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY if linked
 * queue is empty.
 * @note <b>out</b> must be released once done with it.
 */
bool lionfish_concurrent_linked_queue_sr_remove(
        struct lionfish_concurrent_linked_queue_sr *object,
        struct triggerfish_strong **out);

/**
 * @brief Retrieve the item from the front of the queue without removing it.
 * @param [in] object linked queue instance.
 * @param [out] out receive the item in the front of the linked queue.
 * @return On success true, otherwise false if an error has occurred.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OBJECT_IS_NULL if object
 * is <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_OUT_IS_NULL if out is
 * <i>NULL</i>.
 * @throws LIONFISH_CONCURRENT_LINKED_QUEUE_SR_ERROR_QUEUE_IS_EMPTY if linked
 * queue is empty.
 */
bool lionfish_concurrent_linked_queue_sr_peek(
        struct lionfish_concurrent_linked_queue_sr *object,
        const struct triggerfish_strong **out);

#endif /* _LIONFISH_CONCURRENT_LINKED_QUEUE_H_ */
