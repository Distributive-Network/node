
// Temporary fix to https://github.com/nodejs/node/issues/65026
// Adding this dummy file to the sources list for libnode.a

#include "node_version.h"

namespace node {

// This is a dummy variable to ensure that a source is linked into the libnode.a
const char* const dummyNodeVersionString = NODE_VERSION_STRING;

}  // namespace node
