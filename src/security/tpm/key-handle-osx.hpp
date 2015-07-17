/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2015 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_SECURITY_TPM_KEY_HANDLE_OSX_HPP
#define NDN_SECURITY_TPM_KEY_HANDLE_OSX_HPP

#include "key-handle.hpp"
#include "helper-osx.hpp"

#ifndef NDN_CXX_HAVE_OSX_SECURITY
#error "This file should not be compiled ..."
#endif

namespace ndn {
namespace security {
namespace tpm {

class BackEndOsx;
class KeyRefOsx;

/**
 * @brief Abstraction of TPM key handle used by the TPM based on OS X Keychain Service.
 */
class KeyHandleOsx : public KeyHandle
{
public:

  friend class BackEndOsx;

public:
  class Error : public KeyHandle::Error
  {
  public:
    explicit
    Error(const std::string& what)
      : KeyHandle::Error(what)
    {
    }
  };

private:
  KeyHandleOsx(const BackEndOsx& impl, const KeyRefOsx& key);

private:
  virtual ConstBufferPtr
  doSign(DigestAlgorithm digestAlgorithm, const uint8_t* buf, size_t size) const NDN_CXX_DECL_FINAL;

  virtual ConstBufferPtr
  doDecrypt(const uint8_t* cipherText, size_t cipherTextLen) const NDN_CXX_DECL_FINAL;

  virtual ConstBufferPtr
  doDerivePublicKey() const NDN_CXX_DECL_FINAL;

private:
  const BackEndOsx& m_impl;
  KeyRefOsx m_key;
};

} // namespace tpm
} // namespace security
} // namespace ndn

#endif // NDN_SECURITY_TPM_KEY_HANDLE_OSX_HPP
