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

#ifndef NDN_SECURITY_KEY_PARAMS_HPP
#define NDN_SECURITY_KEY_PARAMS_HPP

#include "../common.hpp"
#include "../name-component.hpp"
#include "security-common.hpp"

namespace ndn {

/**
 * @brief Base class of key parameters.
 *
 * Its subclasses are used to store parameters for key generation.
 */
class KeyParams
{
public:
  class Error : public std::runtime_error
  {
  public:
    explicit
    Error(const std::string& what)
      : std::runtime_error(what)
    {
    }
  };

  virtual
  ~KeyParams();

  KeyType
  getKeyType() const
  {
    return m_keyType;
  }

  KeyIdType
  getKeyIdType() const
  {
    return m_keyIdType;
  }

  void
  setKeyId(const name::Component& keyId)
  {
    m_keyId = keyId;
  }

  const name::Component&
  getKeyId() const
  {
    return m_keyId;
  }

protected:
  /**
   * @brief Create a key generation parameter
   *
   * @param keyType Type of the created key
   * @param keyIdType The method to generate the key id, e.g., using key hash (KeyIdType::RANDOM,
   *                  KeyIdType::SHA256), the method MUST NOT be KeyIdType::USER_SPECIFIED, which
   *                  indicates that the key id is pre-specified.
   */
  KeyParams(KeyType keyType, KeyIdType keyIdType);

  /**
   * @brief Create a key generation parameter
   *
   * @param keyType Type of the created key
   * @param keyId The pre-specified key id. The keyIdType will be set to KeyIdType::USER_SPECIFIED.
   *              keyId MUST NOT be the empty component.
   */
  KeyParams(KeyType keyType, const name::Component& keyId);

private:
  KeyType m_keyType;
  KeyIdType m_keyIdType;
  name::Component m_keyId;
};


/// @brief RsaKeyParamInfo is used to initialize a SimplePublicKeyParams template for RSA key.
class RsaKeyParamsInfo
{
public:
  static KeyType
  getType()
  {
    return KeyType::RSA;
  }

  /**
   * @brief check if @p size is qualified.
   *
   * @throw KeyParams::Error if the key size is not supported.
   */
  static uint32_t
  checkKeySize(uint32_t size);

  static uint32_t
  getDefaultSize();
};

/// @brief EcdsaKeyParamInfo is used to initialize a SimplePublicKeyParams template for ECDSA key.
class EcdsaKeyParamsInfo
{
public:
  static KeyType
  getType()
  {
    return KeyType::EC;
  }

  /**
   * @brief check if @p size is qualified.
   *
   * @throw KeyParams::Error if the key size is not supported.
   */
  static uint32_t
  checkKeySize(uint32_t size);

  static uint32_t
  getDefaultSize();
};


/// @brief SimplePublicKeyParams is a template for public keys with only one parameter: size.
template<typename KeyParamsInfo>
class SimplePublicKeyParams : public KeyParams
{
public:
  /// @brief Create key parameter with user specified @p keyId.
  explicit
  SimplePublicKeyParams(const name::Component& keyId,
                        uint32_t size = KeyParamsInfo::getDefaultSize())
    : KeyParams(KeyParamsInfo::getType(), keyId)
  {
    setKeySize(size);
  }

  /**
   * @brief Create key parameter with auto-created keyId.
   *
   * This method is used only if user does not want to maintain the uniqueness of key name.
   * By default, an 8-byte random number will be used as the key Id.
   */
  explicit
  SimplePublicKeyParams(uint32_t size = KeyParamsInfo::getDefaultSize(),
                        KeyIdType keyIdType = KeyIdType::RANDOM)
    : KeyParams(KeyParamsInfo::getType(), keyIdType)
  {
    setKeySize(size);
  }

  uint32_t
  getKeySize() const
  {
    return m_size;
  }

private:
  void
  setKeySize(uint32_t size)
  {
    m_size = KeyParamsInfo::checkKeySize(size);
  }

  uint32_t
  getDefaultKeySize() const
  {
    return KeyParamsInfo::getDefaultSize();
  }

private:
  uint32_t m_size;
};

/// @brief RsaKeyParams carries parameters for RSA key.
typedef SimplePublicKeyParams<RsaKeyParamsInfo> RsaKeyParams;

/// @brief EcdsaKeyParams carries parameters for ECDSA key.
typedef SimplePublicKeyParams<EcdsaKeyParamsInfo> EcdsaKeyParams;


/// @brief AesKeyParamsInfo is used to initialize a SimpleSymmetricKeyParams template for AES key.
class AesKeyParamsInfo
{
public:
  static KeyType
  getType()
  {
    return KeyType::AES;
  }

  /**
   * @brief check if @p size is qualified.
   *
   * @return KeyParams::Error if the key size is not supported.
   */
  static uint32_t
  checkKeySize(uint32_t size);

  static uint32_t
  getDefaultSize();
};


/// @brief SimpleSymmetricKeyParams is a template for symmetric keys with only one parameter: size.
template<typename KeyParamsInfo>
class SimpleSymmetricKeyParams : public KeyParams
{
public:
  /// @brief Create key parameter with user specified @p keyId.
  explicit
  SimpleSymmetricKeyParams(const name::Component& keyId,
                           uint32_t size = KeyParamsInfo::getDefaultSize())
    : KeyParams(KeyParamsInfo::getType(), keyId)
  {
    setKeySize(size);
  }

  uint32_t
  getKeySize() const
  {
    return m_size;
  }

private:
  void
  setKeySize(uint32_t size)
  {
    m_size = KeyParamsInfo::checkKeySize(size);
  }

  uint32_t
  getDefaultKeySize() const
  {
    return KeyParamsInfo::getDefaultSize();
  }

private:
  uint32_t m_size;
};

typedef SimpleSymmetricKeyParams<AesKeyParamsInfo> AesKeyParams;

} // namespace ndn

#endif // NDN_SECURITY_KEY_PARAMS_HPP
