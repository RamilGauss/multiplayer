/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "CryptoAES_Impl.h"

#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include "BL_Debug.h"

namespace nsCryptoAES_Impl
{
  #define CIPHER  ((const EVP_CIPHER*)mCipher)
  #define CONTEXT ((EVP_CIPHER_CTX*)mContext)
}

using namespace nsCryptoAES_Impl;

TCryptoAES_Impl::TCryptoAES_Impl()
{
  mContext = (EVP_CIPHER_CTX*)new EVP_CIPHER_CTX();

	memset( &iv[0], 0, sizeof(iv));
}
//--------------------------------------------------------------------------------
TCryptoAES_Impl::~TCryptoAES_Impl()
{
	EVP_CIPHER_CTX_cleanup(CONTEXT);

  delete mContext;
}
//--------------------------------------------------------------------------------
bool TCryptoAES_Impl::GenerateKey( eCountBits c )
{
  int sizeKey = c / 8;// �� 8 ���
  mKey.SetData(NULL, sizeKey);
  int res = RAND_bytes((unsigned char*)mKey.GetPtr(), mKey.GetSize());

  if(res==0)
    return false;

  // �������� �������� ����������
  switch(c)
  {
    case e128:
      mCipher = (void*)EVP_aes_128_cfb();
      break;
    case e192:
      mCipher = (void*)EVP_aes_192_cfb();
      break;
    case e256:
      mCipher = (void*)EVP_aes_256_cfb();
      break;
  }
  return true;
}
//--------------------------------------------------------------------------------
bool TCryptoAES_Impl::Encrypt( void* pIn, int sizeIn, TContainer& c_out)
{
	// �������� ��������� ���������
	EVP_CIPHER_CTX_init(CONTEXT);
	// �������������� �������� ���������
	int res = EVP_EncryptInit_ex(CONTEXT, CIPHER, NULL, (const unsigned char*)mKey.GetPtr(), iv);
	if(res==-1)
		return false;

  c_out.SetData(NULL, sizeIn);

	int sizeOut;
  // ������� ������
  if(!EVP_EncryptUpdate(CONTEXT, (unsigned char*)c_out.GetPtr(), &sizeOut, (const unsigned char *)pIn, sizeIn))
		return false;
  if(!EVP_EncryptFinal_ex(CONTEXT, (unsigned char*)c_out.GetPtr() + sizeOut, &sizeOut))
		return false;

  return true;
}
//--------------------------------------------------------------------------------
bool TCryptoAES_Impl::Decrypt(void* pIn, int sizeIn, TContainer& c_out)
{
	// �������� ��������� ���������
	EVP_CIPHER_CTX_init(CONTEXT);
	// �������������� �������� ���������
	int res = EVP_DecryptInit_ex(CONTEXT, CIPHER, NULL, (const unsigned char*)mKey.GetPtr(), iv);
	if(res==-1)
		return false;

	c_out.SetData(NULL, sizeIn);
	
	int sizeOut;
	// ��������� ������
	if(!EVP_DecryptUpdate(CONTEXT, (unsigned char*)c_out.GetPtr(), &sizeOut, (const unsigned char *)pIn, sizeIn)) 
		return false;
	// ��������� ������� ������������
	if(!EVP_DecryptFinal_ex(CONTEXT, (unsigned char*)c_out.GetPtr() + sizeOut, &sizeOut)) 
		return false; 

  return true;
}
//--------------------------------------------------------------------------------
bool TCryptoAES_Impl::Decrypt(void* pIn, int sizeIn, TContainerPtr& c_out)
{
  // �������� ��������� ���������
  EVP_CIPHER_CTX_init(CONTEXT);
  // �������������� �������� ���������
  int res = EVP_DecryptInit_ex(CONTEXT, CIPHER, NULL, (const unsigned char*)mKey.GetPtr(), iv);
  if(res==-1)
    return false;

  int sizeOut;
  // ��������� ������
  if(!EVP_DecryptUpdate(CONTEXT, (unsigned char*)c_out.GetPtr(), &sizeOut, (const unsigned char *)pIn, sizeIn)) 
    return false;
  // ��������� ������� ������������
  if(!EVP_DecryptFinal_ex(CONTEXT, (unsigned char*)c_out.GetPtr() + sizeOut, &sizeOut)) 
    return false; 

  return true;
}
//--------------------------------------------------------------------------------
bool TCryptoAES_Impl::GetPublicKey(TContainer& c_out)
{
	if(mKey.GetSize()==0)
		return false;

	c_out.SetData((char*)mKey.GetPtr(), mKey.GetSize());
	return true;
}
//--------------------------------------------------------------------------------
void TCryptoAES_Impl::SetPublicKey(void* pKey, int sizeKey)
{
  mKey.SetData((char*)pKey, sizeKey);

	int sizeCipher = sizeKey*8;
	// �������� �������� ����������
	switch(sizeCipher)
	{
		case e128:
			mCipher = (void*)EVP_aes_128_cfb();
			break;
		case e192:
			mCipher = (void*)EVP_aes_192_cfb();
			break;
		case e256:
			mCipher = (void*)EVP_aes_256_cfb();
			break;
		default:BL_FIX_BUG();
	}

}
//--------------------------------------------------------------------------------

