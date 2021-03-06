/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef NFC_GONK_MESSAGE_H
#define NFC_GONK_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * NFC parcel format.
 *
 * NFC Request:
 *    4 bytes of parcel size. (Big-Endian)
 *    4 byte of request type. Value will be one of NfcRequestType.
 *    (Parcel size - 4) bytes of request data.
 *
 * NFC Response:
 *    4 bytes of parcel size. (Big-Endian)
 *    4 byte of response type.
 *    4 bytes of error code. Value will be one of NfcErrorCode.
 *    (Parcel size - 8) bytes of response data.
 *
 * NFC Notification:
 *    4 bytes of parcel size. (Big-endian)
 *    4 bytes of notification type. Value will one of NfcNotificationType.
 *    (Parcel size - 4) bytes of notification data.
 *
 * Except Parcel size is encoded in Big-Endian, other data will be encoded in
 * Little-Endian.
 */

/**
 * Message types sent from NFCC (NFC Controller)
 */
typedef enum {
  NFCC_MESSAGE_RESPONSE = 0,
  NFCC_MESSAGE_NOTIFICATION = 1
} NFCCMessageType;

/**
 * Error code.
 */
typedef enum {
  NFC_ERROR_SUCCESS = 0,
//TODO Error Code
} NfcErrorCode;

/**
 * Power saving mode.
 */
typedef enum {
  /**
   * No action, a no-op to be able to mimic optional parameters once
   * additional config parameters will be introduced.
   */
  NFC_NO_OP = -1,

  /**
   * Uninitialize NFC chip and turn off.
   */
  NFC_TURN_OFF = 0,

  /**
   * Turn NFC chip on and initialize.
   */
  NFC_TURN_ON = 1,

  /**
   * Enable discovery mode.
   */
  NFC_ENABLE_DISCOVERY = 2,

  /**
   * Disable discovery mode.
   */
  NFC_DISABLE_DISCOVERY = 3,
} NfcHardwareState;

/**
 * NFC technologies.
 */
typedef enum {
  NFC_TECH_UNKNOWN = -1,
  NFC_TECH_NDEF = 0,
  NFC_TECH_NDEF_WRITABLE = 1,
  NFC_TECH_NDEF_FORMATABLE = 2,
  NFC_TECH_P2P = 3,
  NFC_TECH_NFCA = 4,
  NFC_TECH_NFCB = 5,
  NFC_TECH_NFCF = 6,
  NFC_TECH_NFCV = 7,
  NFC_TECH_ISO_DEP = 8,
  NFC_TECH_MIFARE_CLASSIC = 9,
  NFC_TECH_MIFARE_ULTRALIGHT = 10,
  NFC_TECH_BARCODE = 11
} NfcTechnology;

/**
 * NDEF Record
 * @see NFCForum-TS-NDEF, clause 3.2
 */
typedef struct {
  uint32_t tnf;

  uint32_t typeLength;
  uint8_t* type;

  uint32_t idLength;
  uint8_t* id;

  uint32_t payloadLength;
  uint8_t* payload;
} NdefRecordPdu;

/**
 * NDEF Message.
 */
typedef struct {
  uint32_t numRecords;
  NdefRecordPdu* records;
} NdefMessagePdu;

/**
 * Session Id.
 */
typedef uint32_t NfcSessionId;

typedef struct {
  NfcHardwareState hardwareState;
} NfcConfigRequest;

typedef struct {
  /**
   * possible values are : TODO
   */
  uint32_t status;
} NfcConfigResponse;

typedef struct {
  /**
   * The sessionId must correspond to that of a prior
   * NfcNotificationTechDiscovered.
   */
  NfcSessionId sessionId;

  uint8_t technology;
} NfcConnectRequest;

typedef struct {
  /**
   * The sessionId must correspond to that of a prior
   * NfcNotificationTechDiscovered.
   */
  NfcSessionId sessionId;

  //TODO Parcel doesn't have API for boolean, should we use bit-wise for this?
  /**
   * The NDEF is read-only or not.
   */
  uint8_t isReadOnly;

  /**
   * The NDEF can be configured to read-only or not.
   */
  uint8_t canBeMadeReadonly;

  /**
   * Maximum length of the NDEF.
   */
  uint32_t maxNdefLength;
} NfcGetDetailsResponse;

typedef struct {
  /**
   * The sessionId must correspond to that of a prior
   * NfcNotificationTechDiscovered.
   */
  NfcSessionId sessionId;

  /**
   * NDEF Message to be transmitted.
   */
  NdefMessagePdu ndef;
} NfcNdefReadWritePdu;

typedef enum {
  /**
   * NFC_REQUEST_CONFIG
   *
   * Config NFCD options.
   *
   * data is NfcConfigRequest.
   *
   * response is NfcConfigResponse.
   */
  NFC_REQUEST_CONFIG = 0,

  /**
   * NFC_REQUEST_CONNECT
   *
   * Connect to a specific NFC-compatible technology.
   *
   * data is NfcConnectRequest.
   *
   * response is NULL.
   */
  NFC_REQUEST_CONNECT = 1,

  /**
   * NFC_REQUEST_CLOSE
   *
   * Close an open connection that must have been opened with a prior
   * NfcConnectRequest.
   *
   * data is NfcSessionId, which is correlates to a technology that was
   * previously discovered with NFC_NOTIFICATION_TECH_DISCOVERED.
   *
   * response is NULL.
   */
  NFC_REQUEST_CLOSE = 2,

  /**
   * NFC_REQUEST_GET_DETAILS
   *
   * Request the NDEF meta-data. The 'technology' field in
   * NfcNotificationTechDiscovered must include NFC_TECH_NDEF.
   *
   * data is NfcSessionId, which is correlates to a technology that was
   * previously discovered with NFC_NOTIFICATION_TECH_DISCOVERED.
   *
   * respose is NfcGetDetailsResponse.
   */
  NFC_REQUEST_GET_DETAILS = 3,

  /**
   * NFC_REQUEST_READ_NDEF
   *
   * Request the scanned NDEF message. The 'technology' field in
   * NfcNotificationTechDiscovered must include NFC_TECH_NDEF.
   *
   * data is NfcSessionId, which is correlates to a technology that was
   * previously discovered with NFC_NOTIFICATION_TECH_DISCOVERED.
   *
   * response is NfcNdefReadWritePdu.
   */
  NFC_REQUEST_READ_NDEF = 4,

  /**
   * NFC_REQUEST_WRITE_NDEF
   *
   * Write a NDEF message. The 'technology' field in
   * NfcNotificationTechDiscovered must include NFC_TECH_NDEF_WRITABLE or
   * NFC_TECH_P2P.
   *
   * data is NfcNdefReadWritePdu.
   *
   * response is NULL.
   */
  NFC_REQUEST_WRITE_NDEF = 5,

  /**
   * NFC_REQUEST_MAKE_NDEF_READ_ONLY
   *
   * Make the NDEF message is read-only. The 'technology' field in
   * NfcNotificationTechDiscovered must include NFC_TECH_NDEF_WRITABLE.
   *
   * data is NfcSessionId, which is correlates to a technology that was
   * previously discovered with NFC_NOTIFICATION_TECH_DISCOVERED.
   *
   * response is NULL.
   */
  NFC_REQUEST_MAKE_NDEF_READ_ONLY = 6,
} NfcRequestType;

typedef enum {
  NFC_RESPONSE_GENERAL = 1000,

  NFC_RESPONSE_CONFIG = 1001,

  NFC_RESPONSE_READ_NDEF_DETAILS = 1002,

  NFC_RESPONSE_READ_NDEF = 1003,
} NfcResponseType;

typedef struct {
  uint32_t status;
  uint32_t majorVersion;
  uint32_t minorVersion;
} NfcNotificationInitialized;

typedef struct {
  NfcSessionId sessionId;
  uint32_t numOfTechnogies;
  uint8_t* technology;

  uint32_t numOfNdefMsgs;
  NdefMessagePdu* ndef;
} NfcNotificationTechDiscovered;

typedef enum {
  NFC_NOTIFICATION_BASE = 1999,

  /**
   * NFC_NOTIFICATION_INITIALIZED
   *
   * To notify nfcd is initialized after startup.
   *
   * data is NfcNotificationInitialized.
   */
  NFC_NOTIFICATION_INITIALIZED = 2000,

  /**
   * NFC_NOTIFICATION_TECH_DISCOVERED
   *
   * To notify a NFC-compatible technology has been discovered.
   *
   * data is NfcNotificationTechDiscovered.
   */
  NFC_NOTIFICATION_TECH_DISCOVERED = 2001,

  /**
   * NFC_NOTIFICATION_TECH_LOST
   *
   * To notify whenever a NFC-compatible technology is removed from the field of
   * the NFC reader.
   *
   * data is char* sessionId, which is correlates to a technology that was
   * previously discovered with NFC_NOTIFICATION_TECH_DISCOVERED.
   */
  NFC_NOTIFICATION_TECH_LOST = 2002,
} NfcNotificationType;

#ifdef __cplusplus
}
#endif

#endif // NFC_GONK_MESSAGE_H
