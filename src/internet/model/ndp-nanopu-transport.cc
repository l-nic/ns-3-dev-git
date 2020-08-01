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

#include <unordered_map>

#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/nanopu-archt.h"
#include "ndp-nanopu-transport.h"
#include "ns3/ipv4-header.h"

#include "ns3/udp-header.h"

namespace ns3 {
    
NS_LOG_COMPONENT_DEFINE ("NdpNanoPuArcht");

NS_OBJECT_ENSURE_REGISTERED (NdpNanoPuArcht);
    
/******************************************************************************/

TypeId NdpNanoPuArchtIngressPipe::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdpNanoPuArchtIngressPipe")
    .SetParent<Object> ()
    .SetGroupName("Network")
  ;
  return tid;
}

NdpNanoPuArchtIngressPipe::NdpNanoPuArchtIngressPipe (Ptr<NanoPuArchtReassemble> reassemble)
{
  NS_LOG_FUNCTION (this);
    
  m_reassemble = reassemble;
}

NdpNanoPuArchtIngressPipe::~NdpNanoPuArchtIngressPipe ()
{
  NS_LOG_FUNCTION (this);
}
    
bool NdpNanoPuArchtIngressPipe::IngressPipe( Ptr<NetDevice> device, Ptr<const Packet> p, 
                                             uint16_t protocol, const Address &from)
{
  Ptr<Packet> cp = p->Copy ();
  NS_LOG_FUNCTION (this << cp);
  NS_LOG_DEBUG ("At time " <<  Simulator::Now ().GetSeconds () << 
               " NanoPU NDP IngressPipe received a packet of size " << cp->GetSize ());
    
  cp->Print (std::cout);
  std::cout << std::endl;
    
//   Ipv4Header iph;
//   cp->RemoveHeader (iph);
//   NS_LOG_DEBUG ("This is the IP header: " << iph);
//   Ipv4Address src_ip4 = iph.GetSource ();
//   iph.SetSource (iph.GetDestination ());
//   iph.SetDestination (src_ip4);
    
//   UdpHeader udph;
//   cp->RemoveHeader (udph);
//   NS_LOG_DEBUG ("This is the UDP header: " << udph);
//   uint16_t src_port = udph.GetSourcePort ();
//   udph.SetSourcePort (udph.GetDestinationPort ());
//   udph.SetDestinationPort (src_port);
  
//   uint8_t *buffer = new uint8_t[cp->GetSize ()];
//   cp->CopyData(buffer, cp->GetSize ());
//   std::string s = std::string(buffer, buffer+cp->GetSize());
//   NS_LOG_DEBUG ("This is the payload: " << s);
  
//   cp->AddHeader (udph);
//   cp->AddHeader (iph);
     
  /*
   * ASSUMPTION: NanoPU will work with point to point channels, so sending a broadcast
   *             packet on L2 is equivalent to sending a unicast packet.
   * TODO: There should be a clever way of resolving the destination MAC address of the 
   *       switch that is connected to the NanoPU architecture via the m_boundnetdevice
   */
//   Send(cp, m_boundnetdevice->GetBroadcast ());
//   Send(cp, from);
    
  return true;
}
    
/******************************************************************************/
    
TypeId NdpNanoPuArchtEgressPipe::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdpNanoPuArchtEgressPipe")
    .SetParent<Object> ()
    .SetGroupName("Network")
  ;
  return tid;
}

NdpNanoPuArchtEgressPipe::NdpNanoPuArchtEgressPipe (Ptr<NanoPuArcht> nanoPuArcht)
{
  NS_LOG_FUNCTION (this);
    
  m_nanoPuArcht = nanoPuArcht;
}

NdpNanoPuArchtEgressPipe::~NdpNanoPuArchtEgressPipe ()
{
  NS_LOG_FUNCTION (this);
}
    
bool NdpNanoPuArchtEgressPipe::EgressPipe (Ptr<const Packet> p, egressMeta_t meta)
{
  NS_LOG_FUNCTION (this << p);
     
  /*
   * ASSUMPTION: NanoPU will work with point to point channels, so sending a broadcast
   *             packet on L2 is equivalent to sending a unicast packet.
   * TODO: There should be a clever way of resolving the destination MAC address of the 
   *       switch that is connected to the NanoPU architecture via the m_boundnetdevice
   */
//   m_nanoPuArcht->Send(cp, m_boundnetdevice->GetBroadcast ());
//   m_nanoPuArcht->Send(cp, from);
    
  return true;
}
    
/******************************************************************************/
    
TypeId NdpNanoPuArchtPktGen::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdpNanoPuArchtPktGen")
    .SetParent<Object> ()
    .SetGroupName("Network")
  ;
  return tid;
}

NdpNanoPuArchtPktGen::NdpNanoPuArchtPktGen (Ptr<NanoPuArchtArbiter> arbiter)
{
  NS_LOG_FUNCTION (this);
    
  m_arbiter = arbiter;
}

NdpNanoPuArchtPktGen::~NdpNanoPuArchtPktGen ()
{
  NS_LOG_FUNCTION (this);
}

/******************************************************************************/
    
TypeId NdpNanoPuArcht::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NdpNanoPuArcht")
    .SetParent<Object> ()
    .SetGroupName("Network")
  ;
  return tid;
}

NdpNanoPuArcht::NdpNanoPuArcht (Ptr<Node> node, uint16_t maxMessages) : NanoPuArcht (node, maxMessages)
{
  NS_LOG_FUNCTION (this);
  
  m_ingresspipe = CreateObject<NdpNanoPuArchtIngressPipe> (m_reassemble);
  m_egresspipe = CreateObject<NdpNanoPuArchtEgressPipe> (this);
  m_pktgen = CreateObject<NdpNanoPuArchtPktGen> (m_arbiter);
    
  m_arbiter->SetEgressPipe(m_egresspipe);
}

NdpNanoPuArcht::~NdpNanoPuArcht ()
{
  NS_LOG_FUNCTION (this);
}
    
bool NdpNanoPuArcht::EnterIngressPipe( Ptr<NetDevice> device, Ptr<const Packet> p, 
                                    uint16_t protocol, const Address &from)
{
  NS_LOG_FUNCTION (this << p);
    
  m_ingresspipe->IngressPipe (device, p, protocol, from);
      
//   /*
//    * ASSUMPTION: NanoPU will work with point to point channels, so sending a broadcast
//    *             packet on L2 is equivalent to sending a unicast packet.
//    * TODO: There should be a clever way of resolving the destination MAC address of the 
//    *       switch that is connected to the NanoPU architecture via the m_boundnetdevice
//    */
// //   Send(cp, m_boundnetdevice->GetBroadcast ());
//   Send(cp, from);
    
  return true;
}
    
} // namespace ns3