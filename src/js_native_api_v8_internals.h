#ifndef SRC_JS_NATIVE_API_V8_INTERNALS_H_
#define SRC_JS_NATIVE_API_V8_INTERNALS_H_

// The V8 implementation of N-API, including `js_native_api_v8.h` uses certain
// idioms which require definition here. For example, it uses a variant of
// persistent references which need not be reset in the constructor. It is the
// responsibility of this file to define these idioms. Optionally, this file
// may also define `NAPI_VERSION` and set it to the version of N-API to be
// exposed.

// In the case of the Node.js implementation of N-API some of the idioms are
// imported directly from Node.js by including `node_internals.h` below. Others
// are bridged to remove references to the `node` namespace. `node_version.h`,
// included below, defines `NAPI_VERSION`.

#include <v8.h>
#include <cassert>

#define NAPI_ARRAYSIZE(array) \
  (sizeof(array) / sizeof(array[0]))

namespace v8impl {

class RefTracker {
 public:
  RefTracker() {}
  virtual ~RefTracker() {}
  virtual void Finalize(bool isEnvTeardown) {}

  typedef RefTracker RefList;

  inline void Link(RefList* list) {
    prev_ = list;
    next_ = list->next_;
    if (next_ != nullptr) {
      next_->prev_ = this;
    }
    list->next_ = this;
  }

  inline void Unlink() {
    if (prev_ != nullptr) {
      prev_->next_ = next_;
    }
    if (next_ != nullptr) {
      next_->prev_ = prev_;
    }
    prev_ = nullptr;
    next_ = nullptr;
  }

  static void FinalizeAll(RefList* list) {
    while (list->next_ != nullptr) {
      list->next_->Finalize(true);
    }
  }

 private:
  RefList* next_ = nullptr;
  RefList* prev_ = nullptr;
};

template <typename T>
using Persistent = v8::Persistent<T>;

class PersistentToLocal {
public:
    template <class TypeName>
    static inline v8::Local<TypeName> Strong(
        const Persistent<TypeName>& persistent) {
        return *reinterpret_cast<v8::Local<TypeName>*>(
            const_cast<Persistent<TypeName>*>(&persistent));
    }
};

}  // end of namespace v8impl

#ifndef CHECK
#define CHECK(expr) assert(expr)
#endif

#ifndef CHECK_EQ
#define CHECK_EQ(a, b) CHECK((a) == (b))
#endif

#ifndef CHECK_LE
#define CHECK_LE(a, b) CHECK((a) <= (b))
#endif

#endif  // SRC_JS_NATIVE_API_V8_INTERNALS_H_
