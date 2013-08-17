/**
 * @author: Jeff Thompson
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_BINARYXMLELEMENTREADER_H
#define NDN_BINARYXMLELEMENTREADER_H

#include "../errors.h"
#include "binary-xml-structure-decoder.h"
#include "../util/dynamic-uchar-array.h"

#ifdef __cplusplus
extern "C" {
#endif

/** An ndn_ElementListener struct holds a function pointer onReceivedElement.  You can extend this struct with data that
 * will be passed to onReceivedElement.
 */
struct ndn_ElementListener {
  void (*onReceivedElement)(struct ndn_ElementListener *self, unsigned char *element, unsigned int elementLength); /**< see ndn_ElementListener_init */
};

/**
 * Initialize an ndn_ElementListener struct to use the onReceivedElement function pointer.
 * @param self pointer to the ndn_ElementListener struct
 * @param onReceivedElement pointer to a function which is called when an entire binary XML element is received.
 * self is the pointer to this ndn_ElementListener struct.  See ndn_BinaryXmlElementReader_onReceivedData.
 */
static inline void ndn_ElementListener_init
  (struct ndn_ElementListener *self, void (*onReceivedElement)(struct ndn_ElementListener *self, unsigned char *element, unsigned int elementLength))
{
  self->onReceivedElement = onReceivedElement;
}
  
/**
 * A BinaryXmlElementReader lets you call ndn_BinaryXmlElementReader_onReceivedData multiple times which uses an
 * ndn_BinaryXmlStructureDecoder to detect the end of a binary XML element and calls
 * (*elementListener->onReceivedElement)(element, elementLength) with the element. 
 * This handles the case where a single call to onReceivedData may contain multiple elements.
 */
struct ndn_BinaryXmlElementReader {
  struct ndn_ElementListener *elementListener;
  struct ndn_BinaryXmlStructureDecoder structureDecoder;
  int usePartialData;
  struct ndn_DynamicUCharArray partialData;
  unsigned int partialDataLength;
};

/**
 * Initialize an ndn_BinaryXmlElementReader struct with the elementListener and a buffer for saving partial data.
 * @param self pointer to the ndn_BinaryXmlElementReader struct
 * @param elementListener pointer to the ndn_ElementListener used by ndn_BinaryXmlElementReader_onReceivedData.
 * @param buffer the allocated buffer.  If reallocFunction is null, this should be large enough to save a full element, perhaps 8000 bytes.
 * @param bufferLength the length of the buffer
 * @param reallocFunction see ndn_DynamicUCharArray_ensureLength.  This may be 0.
 */
static inline void ndn_BinaryXmlElementReader_init
  (struct ndn_BinaryXmlElementReader *self, struct ndn_ElementListener *elementListener,
   unsigned char *buffer, unsigned int bufferLength, unsigned char * (*reallocFunction)(struct ndn_DynamicUCharArray *self, unsigned char *, unsigned int))
{
  self->elementListener = elementListener;
  ndn_BinaryXmlStructureDecoder_init(&self->structureDecoder);
  self->usePartialData = 0;
  ndn_DynamicUCharArray_init(&self->partialData, buffer, bufferLength, reallocFunction);
}

/**
 * Continue to read binary XML data until the end of an element, then call (*elementListener->onReceivedElement)(element, elementLength).
 * The buffer passed to onReceivedElement is only valid during this call.  If you need the data later, you must copy.
 * @param self pointer to the ndn_BinaryXmlElementReader struct
 * @param data pointer to the buffer with the binary XML bytes
 * @param dataLength length of data
 * @return 0 for success, else an error code
 */
ndn_Error ndn_BinaryXmlElementReader_onReceivedData
  (struct ndn_BinaryXmlElementReader *self, unsigned char *data, unsigned int dataLength);

#ifdef __cplusplus
}
#endif

#endif