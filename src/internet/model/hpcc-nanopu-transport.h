/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2021 Stanford University
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

#ifndef HPCC_NANOPU_TRANSPORT_H
#define HPCC_NANOPU_TRANSPORT_H

#include <unordered_map>

#include "ns3/object.h"
#include "ns3/nanopu-archt.h"
#include "ns3/int-header.h"

// Define module delay in nano seconds
#define HPCC_INGRESS_PIPE_DELAY 5
#define HPCC_EGRESS_PIPE_DELAY 1

namespace ns3 {

/**
 * \ingroup nanopu-archt
 *
 * \brief Programmable Packet Generator Architecture for NanoPU with HPCC Transport
 *
 */
class HpccNanoPuArchtPktGen : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  HpccNanoPuArchtPktGen (Ptr<NanoPuArcht> nanoPuArcht);
  ~HpccNanoPuArchtPktGen (void);
  
  void CtrlPktEvent (Ipv4Address dstIp, uint16_t dstPort, uint16_t srcPort,
                     uint16_t txMsgId, uint16_t pktOffset, uint16_t msgLen,
                     IntHeader receivedIntHeader);
  
protected:
  Ptr<NanoPuArcht> m_nanoPuArcht; //!< the archt itself to send generated packets
};
 
/******************************************************************************/
 
/**
 * \ingroup nanopu-archt
 *
 * \brief Ingress Pipeline Architecture for NanoPU with HPCC Transport
 *
 */
class HpccNanoPuArchtIngressPipe : public Object
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  HpccNanoPuArchtIngressPipe (Ptr<NanoPuArchtReassemble> reassemble,
                              Ptr<NanoPuArchtPacketize> packetize,
                              Ptr<HpccNanoPuArchtPktGen> pktgen,
                              double baseRtt, uint32_t mtu,
                              uint16_t initCredit, uint32_t winAI,
                              double utilFac, uint16_t maxStage);
  ~HpccNanoPuArchtIngressPipe (void);
  
  uint16_t ComputeNumPkts (uint32_t winSizeBytes);
  
  double MeasureInflight (uint16_t txMsgId, IntHeader intHdr);
  
  uint32_t ComputeWind (uint16_t txMsgId, double utilization, bool updateWc);
  
  bool IngressPipe (Ptr<NetDevice> device, Ptr<const Packet> p, 
                    uint16_t protocol, const Address &from);
  
protected:

    Ptr<NanoPuArchtReassemble> m_reassemble; //!< the reassembly buffer of the architecture
    Ptr<NanoPuArchtPacketize> m_packetize; //!< the packetization buffer of the architecture
    Ptr<HpccNanoPuArchtPktGen> m_pktgen; //!< the programmable packet generator of the NDP architecture
    
    double m_baseRTT;      //!< The base propagation RTT in seconds.
    uint32_t m_mtu;        //!< The MTU size of the network
    uint16_t m_initCredit; //!< The initial number of packets allowed to be sent (ie. BDP in packets)
    uint32_t m_winAI;      //!< Additive increase factor in Bytes
    double m_utilFac;      //!< Utilization Factor (defined as \eta in HPCC paper)
    uint16_t m_maxStage;   //!< Maximum number of stages before window is updated wrt. utilization
    
//     std::unordered_map<uint16_t, bool> m_validStates;        //!< State to track validity {txMsgId => state valid or not}
    std::unordered_map<uint16_t, uint16_t> m_credits;        //!< State to track credit {txMsgId => max seqNo for TX}
    std::unordered_map<uint16_t, uint16_t> m_ackNos;         //!< State to track ackNo {txMsgId => ack No}
    std::unordered_map<uint16_t, uint32_t> m_winSizes;       //!< State to track W^c {txMsgId => Window Size in Bytes}
    std::unordered_map<uint16_t, uint16_t> m_lastUpdateSeqs; //!< State to track seqNo {txMsgId => Last Update Seq}
    std::unordered_map<uint16_t, uint16_t> m_incStages;      //!< State to track incStage {txMsgId => inc Stage}
    std::unordered_map<uint16_t, IntHeader> m_prevIntHdrs;   //!< State to track INT vector {txMsgId => Prev INT hdr}
    std::unordered_map<uint16_t, double> m_utilizations;     //!< State to track utilization {txMsgId => U}
};
 
/******************************************************************************/
    
/**
 * \ingroup nanopu-archt
 *
 * \brief Egress Pipeline Architecture for NanoPU with HPCC Transport
 *
 */
class HpccNanoPuArchtEgressPipe : public NanoPuArchtEgressPipe
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  HpccNanoPuArchtEgressPipe (Ptr<NanoPuArcht> nanoPuArcht);
  ~HpccNanoPuArchtEgressPipe (void);
  
  void EgressPipe (Ptr<const Packet> p, egressMeta_t meta);
  
protected:
  Ptr<NanoPuArcht> m_nanoPuArcht; //!< the archt itself to send packets
};
 
/******************************************************************************/
    
/**
 * \ingroup nanopu-archt
 *
 * \brief Transport Specific Architecture for devices to replace internet and transport layers.
 *        This version of the architecture implements specifically the HPCC transport protocol.
 *
 */
class HpccNanoPuArcht : public NanoPuArcht
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  
  HpccNanoPuArcht ();
  virtual ~HpccNanoPuArcht (void);
  
  void AggregateIntoDevice (Ptr<NetDevice> device);
  
  /**
   * \brief Implements programmable ingress pipeline architecture.
   *
   * \param device Pointer to NetDevice of desired interface
   * \param p Pointer to the arriving packet
   * \param protocol L3 protocol of the incomming packet (Can assume IPv4 for nanoPU)
   * \param from The L2 source address of the incoming packet 
   * \returns boolean to check successful completion of the packet processing
   */
  bool EnterIngressPipe (Ptr<NetDevice> device, Ptr<const Packet> p, 
                         uint16_t protocol, const Address &from);

protected:

  Ptr<HpccNanoPuArchtIngressPipe> m_ingresspipe; //!< the programmable ingress pipeline for the archt
  Ptr<HpccNanoPuArchtEgressPipe> m_egresspipe; //!< the programmable egress pipeline for the archt
  Ptr<HpccNanoPuArchtPktGen> m_pktgen; //!< the programmable packet generator for the archt
  
  double m_baseRtt;      //!< The base propagation RTT in seconds.
  uint32_t m_winAi;      //!< Additive increase factor in Bytes
  double m_utilFac;      //!< Utilization Factor (defined as \eta in HPCC paper)
  uint16_t m_maxStage;   //!< Maximum number of stages before window is updated wrt. utilization
};   

} // namespace ns3

#endif /* HPCC_NANOPU_TRANSPORT */