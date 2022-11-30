#include "placer.hpp"

void Placer::splitString(std::string str, std::vector<std::string>& vec){
    std::vector<std::string> words;
    int tmp = 0, i = 0, width = 0;
    std::string word;

    while( i < str.length() ){
        while( (str[i] == ' ') || (str[i] == '\t') || (str[i] == '\r') ){
            i++;
        }
        tmp = i;
        width = 0;
        while( (str[i] != ' ') && (str[i] != '\t') && (str[i] != '\r') && (i < str.length()) ){
            i++;
            width++;
        }
        word = str.substr(tmp,width);
        words.push_back(word);
    }
    vec = words;
}

void Placer::calTtlArea(){
    m_ttlArea = 0;
    for(int i = 0; i < m_vBlockList.size(); ++i){
        m_ttlArea += m_vBlockList[i]->getArea();
    }
}

bool Placer::isTimeout(const time_t& start) const{
    time_t end = clock();
    double t_used = (double)(end - start) / CLOCKS_PER_SEC;
    
    if(t_used > TIME_LIMIT){
        std::cerr << "> Warning: Break by timeout (" << TIME_LIMIT << " seconds)." << std::endl;
        return true;
    }
    else{
        return false;
    }
}


void Placer::clearLists(){
    for(int  i = 0; i < m_mainBlockList.size(); ++i){
        delete m_mainBlockList[i];
    }      

    m_vBlockList.clear();    
    m_vPinList.clear();
    m_vNetList.clear();
       
       /*
       for(map<string, Btree*>::const_iterator it = _bstar.begin();
       it != _bstar.end(); ++it)
          delete it->second;
       _bstar.clear();
        */
    m_mainBlockList.clear();
    

    /*
     if(_hbtree)delete _hbtree;
     if(_recoverMsg) delete _recoverMsg;
    */
}

void Placer::clearSAList(){
    for(int i = 0; i < m_saBlockList.size(); ++i){
        m_saBlockList[i] = 0;
    }
    for(int i = 0; i < m_saSymList.size(); ++i){
        m_saSymList[i] = 0;
    }

    if(!m_saBlockHash.empty()){
        for(auto iter = m_saBlockHash.begin(); iter != m_saBlockHash.end(); ++iter){
            delete iter->second;
        }
    }
    m_saBlockHash.clear();

    for(int i = 0; i < m_saNewBlockList.size(); ++i){
        delete m_saNewBlockList[i];
    }

    m_saSymList.clear();
    m_saBlockList.clear();
    m_saNewBlockList.clear();
}