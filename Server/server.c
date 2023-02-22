#include <stdbool.h>
#include "client_context.h"
#include "server_utils.h"

int main()
{
    setup_sig_handle();
    setup_db();
    setup_socket();
    setup_thread_pool();

    // Run until interrupted
    // it can also run as one of the listening process
    // but we don't want the main trhead to exit so we will leave it
    // as an idle Task
    idle_task();

    return 0;
}
