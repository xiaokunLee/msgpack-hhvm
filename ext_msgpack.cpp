/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2013 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/base-includes.h"
#include "msgpack-variant.h"

#define MSGPACK_HHVM_VERSION "0.0.1"

namespace HPHP {

static String HHVM_FUNCTION(msgpack_pack, const Variant &var) {
  msgpack::sbuffer sbuf;
  msgpack::pack(sbuf, var);

  return String(sbuf.data(), sbuf.size(), AttachString);
}

static Variant HHVM_FUNCTION(msgpack_unpack, const String &pack) {
  msgpack::zone zone;
  msgpack::object obj;
  msgpack::unpack(pack.c_str(), pack.size(), NULL, &zone, &obj);

  Variant v;

  obj.convert(&v);

  return v;
}

static class MsgpackExtension : public Extension {
 public:
  MsgpackExtension() : Extension("msgpack", MSGPACK_HHVM_VERSION) {}
  virtual void moduleInit() {
    HHVM_FE(msgpack_pack);
    HHVM_FE(msgpack_unpack);

    HHVM_FALIAS(msgpack_serialize, msgpack_pack);
    HHVM_FALIAS(msgpack_unserialize, msgpack_unpack);

    loadSystemlib();
  }
} s_msgpack_extension;

HHVM_GET_MODULE(msgpack);

} // namespace HPHP
