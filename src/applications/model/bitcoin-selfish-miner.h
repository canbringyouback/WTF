#ifndef BITCOIN_SELFISH_MINER_H
#define BITCOIN_SELFISH_MINER_H

#include "bitcoin-miner.h"


namespace ns3 {

class Address;
class Socket;
class Packet;

enum ForkType
{
  IRRELEVANT,
  RELEVANT,
  ACTIVE
};

enum Action
{
  ADOPT,
  OVERRIDE,
  MATCH,
  WAIT,
  EXIT,
  ERROR
};

const char* getForkType(enum ForkType m);
const char* getAction(enum Action m);

class BitcoinSelfishMiner : public BitcoinMiner 
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  BitcoinSelfishMiner ();

  virtual ~BitcoinSelfishMiner (void);

protected:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop

  virtual void DoDispose (void);

  virtual void MineBlock (void);

  virtual void ReceivedHigherBlock(const Block &newBlock);	//Called for blocks with better score(height). Remove m_nextMiningEvent and call MineBlock again.

  /**
   * \brief Called when a new block non-orphan block is received
   * \param newBlock the newly received block
   */
  virtual void ReceiveBlock(const Block &newBlock);	
  
  /**
   * \brief Called when a new block non-orphan block is received
   * \param newBlock the newly received block
   */
  void ReleaseChain(std::vector<Block> blocks);	
  
  enum Action ReadActionMatrix(enum ForkType f, int la, int lh);
  
  bool       m_attackFinished;
  int        m_la;
  int        m_lh;
  Block      m_honestNetworkTopBlock;
  Block      m_attackerTopBlock;
  int        m_maxAttackBlocks;
  enum ForkType m_forkType;

//Stale block rate = 1%  
  char m_decisionMatrix[3][20][20] = 
	{{{'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w'}}, 
	{{'*', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}}, 
	{{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}}};

	
/* //Stale block rate = 10%  
  char m_decisionMatrix[3][20][20] = 
	{{{'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'e', 'e', 'e', 'e', 'e', 'e', 'e', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', 'w', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'e', 'w'}}, 
	{{'*', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'w', 'a'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w', 'a'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', 'w'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}}, 
	{{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
	{'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'}}}; */

};

} // namespace ns3

#endif /* BITCOIN_SELFISH_MINER_H */
