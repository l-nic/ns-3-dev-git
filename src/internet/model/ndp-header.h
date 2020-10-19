/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 Stanford University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Serhat Arslan <sarslan@stanford.edu>
 */

#ifndef NDP_HEADER_H
#define NDP_HEADER_H

#include "ns3/header.h"

namespace ns3 {
/**
 * \ingroup ndp
 * \brief Packet header for NDP Transport packets
 *
 * This class has fields corresponding to those in a network NDP header
 * (transport protocol) as well as methods for serialization
 * to and deserialization from a byte buffer.
 */
class NdpHeader : public Header 
{
public:
  /**
   * \brief Constructor
   *
   * Creates a null header
   */
  NdpHeader ();
  ~NdpHeader ();
  
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  
  /**
   * \param port The source port for this NdpHeader
   */
  void SetSrcPort (uint16_t port);
  /**
   * \return The source port for this NdpHeader
   */
  uint16_t GetSrcPort (void) const;
  
  /**
   * \param port The destination port for this NdpHeader
   */
  void SetDstPort (uint16_t port);
  /**
   * \return the destination port for this NdpHeader
   */
  uint16_t GetDstPort (void) const;
  
  /**
   * \param txMsgId The TX message ID for this NdpHeader
   */
  void SetTxMsgId (uint16_t txMsgId);
  /**
   * \return The source port for this NdpHeader
   */
  uint16_t GetTxMsgId (void) const;
  
  /**
   * \brief Set flags of the header
   * \param flags the flags for this NdpHeader
   */
  void SetFlags (uint8_t flags);
  /**
   * \brief Get the flags
   * \return the flags for this NdpHeader
   */
  uint8_t GetFlags () const;
  
  /**
   * \param msgLen The message length for this NdpHeader in packets
   */
  void SetMsgLen (uint16_t msgLen);
  /**
   * \return The message length for this NdpHeader in packets
   */
  uint16_t GetMsgLen (void) const;
  
  /**
   * \param pktOffset The packet identifier for this NdpHeader in number of packets 
   */
  void SetPktOffset (uint16_t pktOffset);
  /**
   * \return The packet identifier for this NdpHeader in number of packets 
   */
  uint16_t GetPktOffset (void) const;
  
  /**
   * \param pullOffset The pull identifier for this NdpHeader in number of packets 
   */
  void SetPullOffset (uint16_t pullOffset);
  /**
   * \return The pull identifier for this NdpHeader in number of packets 
   */
  uint16_t GetPullOffset (void) const;
  
  /**
   * \param payloadSize The payload size for this NdpHeader in bytes
   */
  void SetPayloadSize (uint16_t payloadSize);
  /**
   * \return The payload size for this NdpHeader in bytes
   */
  uint16_t GetPayloadSize (void) const;
  
  /**
   * \brief Converts an integer into a human readable list of NDP flags
   *
   * \param flags Bitfield of NDP flags to convert to a readable string
   * \param delimiter String to insert between flags
   *
   * \return the generated string
   **/
  static std::string FlagsToString (uint8_t flags, const std::string& delimiter = "|");
  
  /**
   * \brief NDP flag field values
   */
  typedef enum Flags_t
  {
    DATA = 1,  //!< DATA Packet
    ACK  = 2,  //!< ACK
    NACK  = 4, //!< NACK
    PULL  = 8, //!< PULL
    CHOP  = 16, //!< CHOP
    F1  = 32,  //!< Empty for future reference
    F2  = 64,  //!< Empty for future reference
    F3  = 128   //!< Empty for future reference
  } Flags_t;
  
private:

  uint16_t m_srcPort;      //!< Source port
  uint16_t m_dstPort;      //!< Destination port
  uint16_t m_txMsgId;      //!< ID generated by the sender
  uint8_t m_flags;         //!< Flags
  uint16_t m_msgLen;       //!< Length of the message in packets
  uint16_t m_pktOffset;    //!< Similar to seq number (in packets)
  uint16_t m_pullOffset;   //!< Similar to ack number (in packets)
  uint16_t m_payloadSize;  //!< Payload size
  
};
} // namespace ns3
#endif /* NDP_HEADER */