#!/usr/bin/env python3

"""
    This file is a ported version of the C example bundled with NEAT.
    Note that the NEAT bindings currently require Python 2.7 or larger.
    Python 3 will not work.
"""

from neat import *
import sys
import ctypes

def on_readable(ops):
    bytes_read = 0
    buffer = ctypes.create_string_buffer(32)
    if (neat_read(ops.ctx, ops.flow, buffer, 31, bytes_read, None, 0) == NEAT_OK):
        print("Read {} bytes:\n{}".format(bytes_read, buffer))
    return NEAT_OK

def on_writable(ops):
    message = "Hello, this is NEAT!"
    neat_write(ops.ctx, ops.flow, message, 20, None, 0)
    return NEAT_OK

def on_all_written(ops):
    neat_close(ops.ctx, ops.flow)
    return NEAT_OK

def on_connected(ops):
    ops.on_writable = on_writable
    ops.on_all_written = on_all_written
    neat_set_operations(ops.ctx, ops.flow, ops)
    return NEAT_OK

if __name__ == "__main__":

    ctx  = neat_init_ctx()
    flow = neat_new_flow(ctx)
    ops  = neat_flow_operations()

    ops.on_readable = on_readable
    ops.on_connected = on_connected
    neat_set_operations(ctx, flow, ops)

    if (neat_accept(ctx, flow, 5000, None, 0)):
        sys.exit("neat_accept failed")

    neat_start_event_loop(ctx, NEAT_RUN_DEFAULT)
