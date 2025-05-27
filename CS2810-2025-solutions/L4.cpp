#include<bits/stdc++.h>
using namespace std;

class QNS
{
    private:
        int powerLevel;
        int durability;
        int energyStorage;
        int heatLevel;
    public:
        QNS()
        {
            powerLevel=1000;
            durability=500;
            energyStorage=300;
            heatLevel=0;
        }
        
        QNS(int _powerLevel,int _durability,int _energyStorage,int _heatLevel)
        {
            powerLevel=_powerLevel;
            durability=_durability;
            energyStorage=_energyStorage;
            heatLevel=_heatLevel;
            
            if(powerLevel>5000) powerLevel=5000;  //powerLevel capped at 5000
        }
        
        QNS(const QNS& obj)
        {
            powerLevel=obj.powerLevel; //assuming powerBoost gets added to power of the given suit
            durability=obj.durability;
            energyStorage=obj.energyStorage;
            heatLevel=obj.heatLevel;
        }
        
        
        
        QNS operator+(const QNS& obj)
        {
            QNS res(this->powerLevel+obj.energyStorage, this->durability+obj.durability, this->energyStorage+obj.powerLevel, this->heatLevel);
            if(res.getPowerLevel()>5000) res.setPowerLevel(5000);    
            return res;
        }
        
        QNS operator-(const int& x)
        {
            QNS res(this->powerLevel, this->durability-x, this->energyStorage+x, this->heatLevel+x);
            return res;
        }
        
        QNS operator*(const int& x) //give a power Boost
        {
            QNS res( this->powerLevel + (this->powerLevel*x*1.0f)/100, this->durability, this->energyStorage+5*x, this->heatLevel+x );
            return res;
        }
        
        QNS operator/(const int& x) //activates cooling system
        {
            QNS res(this->powerLevel, this->durability+x, this->energyStorage, this->heatLevel-x);
            if( res.getHeatLevel()<0 ) res.setHeatLevel(0);
            return res;    
        }
        
        void boostPower(int factor)
        {
            QNS res=(*this)*factor; //multiply operator has to be used to invoke power boost.
            this->powerLevel=res.powerLevel;
            this->durability=res.durability;
            this->energyStorage=res.energyStorage;
            this->heatLevel=res.heatLevel;
        }
        
        void boostPower(QNS otherSuit)
        {
            QNS res=(*this)+otherSuit;
            
            this->powerLevel=res.powerLevel;
            this->durability=res.durability;
            this->energyStorage=res.energyStorage;
            this->heatLevel=res.heatLevel;
        }
        
        bool operator==(const QNS& obj)
        {
            if( ((this->powerLevel)==obj.powerLevel) && ((this->durability)==obj.durability) ) return true;
            else return false;
        }
        
        bool operator<(const QNS& obj)
        {
            if( (this->powerLevel+this->durability) > (obj.powerLevel+obj.durability) ) return true;
            else return false;
        }
        
        int getHeatLevel()
        {
            return heatLevel;
        }
        
        int getDurability()
        {
            return durability;
        }    
        
        int getPowerLevel()
        {
            return powerLevel;
        }
        
        int getEnergyStorage()
        {
            return energyStorage;
        }
        
        void setPowerLevel(int _powerLevel)
        {
            powerLevel=_powerLevel;
        }
        
        void setEnergyStorage(int _energyStorage)
        {
            energyStorage=_energyStorage;
        }
        
        void setDurability(int _durability)
        {
            durability=_durability;
        }
        
        void setHeatLevel(int _heatLevel)
        {
            heatLevel=_heatLevel;
        }
};

//these are global variables
vector<QNS> suits;
int nextSuitIndex=0;

class Avenger 
{
    private:
        string name;
        QNS suit;
        int attackStrength;
    
    public:
        Avenger()
        {
            name="";
            suit=QNS();
            attackStrength=0;
        }
        
        Avenger(string _name,QNS& _suit,int _attackStrength)
        {
            name=_name;
            suit=_suit;
            attackStrength=_attackStrength;    
        }
        
        Avenger(const Avenger& obj)
        {
            name=obj.name;
            suit=obj.suit;
            attackStrength=obj.attackStrength;
        }
        void attack(Avenger& enemy) //this function is invoked only when attacker can make an attack and enemy's suit is not destroyed.
        {    
            QNS& enemySuit=enemy.getSuit();
            QNS res=enemySuit-attackStrength;
            enemySuit=res;
        }
        
        void upgradeSuit() //this function is called only when there is a suit available to upgrade
        {
            suit=suit+suits[nextSuitIndex++];
        }
        
        void repair(int x)
        {
            QNS modifiedSuit=suit/x;
            suit=modifiedSuit;            
        }
        
        void printStatus()
        {
            cout<<name<<" "<<suit.getPowerLevel()<<" "<<suit.getDurability()<<" "<<suit.getEnergyStorage()<<" "<<suit.getHeatLevel()<<"\n";
        }
        
        QNS& getSuit()
        {
            return suit;
        }
};


map<string,Avenger> heroMap;
map<string,Avenger> enemyMap;

class Battle 
{
    private:
    vector<Avenger> heroes;
    vector<Avenger> enemies;
    vector<string> battleLog;
    
    public:
    void startBattle()
    {
        int k,n,m;
        cin>>k>>n>>m;
        
        for(int i=0;i<k;i++)
        {
            int _powerLevel,_durability,_energyStorage,_heatLevel;
            cin>>_powerLevel>>_durability>>_energyStorage>>_heatLevel;
            suits.push_back(QNS(_powerLevel,_durability,_energyStorage,_heatLevel));
        }
        
        for(int i=0;i<n;i++)
        {
            string name;
            int _attackStrength;
            cin>>name>>_attackStrength;
            
            if(nextSuitIndex==suits.size())
            {
                cout<<name<<" is out of fight\n"; //there is no free suit for this hero
            }
            else 
            {
                Avenger _hero(name,suits[nextSuitIndex++],_attackStrength);
                heroMap[name]=_hero;                
            }

        }
        
        for(int i=0;i<m;i++)
        {
            string name;
            int _attackStrength;
            cin>>name>>_attackStrength;
            
            if(nextSuitIndex==suits.size())
            {
                cout<<name<<" is out of fight\n"; //there is no free suit for this enemy
            }
            else
            {
                Avenger _enemy(name,suits[nextSuitIndex++],_attackStrength);
                enemyMap[name]=_enemy;
            }
        }
        
        string command;
        cin>>command; //this will always be "BATTLEBegin" so just ignore it
        
        while(true)
        {
            cin>>command;
            if(command=="End") break;
            else if(command=="Attack")
            {
                string name1,name2;
                cin>>name1>>name2;
                
                Avenger& avenger1=(heroMap.find(name1)!=heroMap.end())?heroMap[name1]:enemyMap[name1];
                Avenger& avenger2=(heroMap.find(name2)!=heroMap.end())?heroMap[name2]:enemyMap[name2];
                QNS& suit1=avenger1.getSuit();
                QNS& suit2=avenger2.getSuit();
                
                
                if(suit1.getDurability()>0&&suit1.getHeatLevel()<=500&&suit2.getDurability()>0)
                {
                    //now we are able to make an attack
                    battleLog.push_back(name1+" attacks "+name2);
                    avenger1.attack(avenger2);
                    suit2=avenger2.getSuit();
                    if(suit2.getDurability()<0) battleLog.push_back(name2+" suit destroyed");
                    else if(suit2.getHeatLevel()>500) battleLog.push_back(name2+" suit overheated");
                }
            }
            else if(command=="Repair")
            {
                string name;
                int x;
                cin>>name>>x;
                Avenger& avenger1=(heroMap.find(name)!=heroMap.end())?heroMap[name]:enemyMap[name];
                battleLog.push_back(name+" repaired");
                avenger1.repair(x);
            }
            else if(command=="BoostPowerByFactor")
            {
                string name;
                int y;
                cin>>name>>y;
                Avenger& avenger1=(heroMap.find(name)!=heroMap.end())?heroMap[name]:enemyMap[name];
                
                battleLog.push_back(name+" boosted");
                avenger1.getSuit().boostPower(y);
                QNS& suit1=avenger1.getSuit();
                
                if(suit1.getHeatLevel()>500) battleLog.push_back(name+" suit overheated");
            }
            else if(command=="BoostPower")
            {
                string name;
                int p,d,e,h;
                cin>>name>>p>>d>>e>>h;
                
                Avenger& avenger1=(heroMap.find(name)!=heroMap.end())?heroMap[name]:enemyMap[name];
                QNS newSuit(p,d,e,h);
                battleLog.push_back(name+" boosted");
                
                avenger1.getSuit().boostPower(newSuit);
                QNS& suit1=avenger1.getSuit();
                if(suit1.getHeatLevel()>500) battleLog.push_back(name+" suit overheated");
            }
            else if(command=="AvengerStatus")
            {
                string name;
                cin>>name;
                Avenger& avenger1=(heroMap.find(name)!=heroMap.end())?heroMap[name]:enemyMap[name];
                avenger1.printStatus();
            }
            else if(command=="PrintBattleLog")
            {
                printBattleLog();
            }
            else if(command=="BattleStatus")
            {
                int res=result();
                if(res==0) cout<<"tie\n";
                else if(res==1) cout<<"heroes are winning\n";
                else if(res==-1) cout<<"enemies are winning\n";
            }
            else if(command=="Upgrade")
            {
                string name;
                cin>>name;
                
                Avenger& avenger1=(heroMap.find(name)!=heroMap.end())?heroMap[name]:enemyMap[name];
                
                if(nextSuitIndex==suits.size()) 
                {
                    battleLog.push_back(name+" upgrade Fail");
                }
                else
                {
                    avenger1.upgradeSuit();
                    battleLog.push_back(name+" upgraded");    
                }
            }
        }
    }
    
    
    void printBattleLog()
    {
        for(auto events:battleLog) cout<<events<<"\n";
    }
    
    int result()
    {
        long long heroScore=0,enemyScore=0;
                
        for(auto& p:heroMap)
        {
            Avenger& a=p.second;
            if( a.getSuit().getDurability()>0) heroScore+=(a.getSuit().getPowerLevel()+a.getSuit().getDurability());
        }
        
        for(auto& p:enemyMap)
        {
            Avenger& a=p.second;
            if( a.getSuit().getDurability()>0) enemyScore+=(a.getSuit().getPowerLevel()+a.getSuit().getDurability());    
        }
        if(heroScore==enemyScore) return 0;
        else return (heroScore<enemyScore)?-1:1;
    }
};


int main()
{
    Battle b;
    
    b.startBattle();
    return 0;
}
