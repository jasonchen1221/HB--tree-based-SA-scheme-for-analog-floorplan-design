#include "placer.hpp"

using std::cout; using std::endl;

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

double Placer::getTime(const time_t& start) const{
    time_t end = clock();
    double t_used = (double)(end - start) / CLOCKS_PER_SEC;
    return t_used;
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

void Placer::printBlockList() const{
    cout << "BlockList size: " << m_vBlockList.size() << endl;
    for(int i = 0; i < m_vBlockList.size(); ++i){
        cout << *m_vBlockList[i] << endl;
    }
}

void Placer::printPinList() const{
    cout << "PinList size: " << m_vPinList.size() << endl;
    for(int i = 0; i < m_vPinList.size(); ++i){
        cout << *m_vPinList[i] << endl;
    }
}

void Placer::printNetList() const{
    cout << "NetList size: " << m_vNetList.size() << endl;
    for(int i = 0; i < m_vNetList.size(); ++i){
        cout << "[" << i << "]" << endl;
        cout << *m_vNetList[i] << endl;
    }
}

void Placer::printRnd() const{
    int a = 0, b = 0, c = 0, d = 0;
    for(int i = 0; i < 10000000; ++i){
        double k = getRnd();
        if(k < 0.25)        a++;
        else if(k < 0.5)    b++;
        else if(k < 0.75)   c++;
        else                d++;
    }
    cout << a << " " << b << " " << c << " " << d << endl;
 }

void Placer::clear(){
    for(int i = 0, size = m_vBlockList.size(); i < size; ++i){
        delete m_vBlockList[i];
    }

    for(int i = 0, size = m_vPinList.size(); i < size; ++i){
        delete m_vPinList[i];
    }

    for(int i = 0, size = m_vNetList.size(); i < size; ++i){
        delete m_vNetList[i];
    }

    if(m_BTree != nullptr) delete m_BTree;
    if(m_recoverMsg != nullptr) delete m_recoverMsg;
}

double  Placer::getRnd() const{
    // only return 0.0000... ~ 0.9999...
    return ((double)(rand() % RANDOM_RANGE) / RANDOM_RANGE);
}

void Placer::calTTLArea(){
    m_totalArea = 0;
    for(int i = 0; i < m_vBlockList.size(); ++i){
        m_totalArea += m_vBlockList[i]->getArea();
    }
}

size_t Placer::getHashSize(size_t s) const{
    if (s < 8) return 7;
    if (s < 16) return 13;
    if (s < 32) return 31;
    if (s < 64) return 61;
    if (s < 128) return 127;
    if (s < 512) return 509;
    if (s < 2048) return 1499;
    if (s < 8192) return 4999;
    if (s < 32768) return 13999;
    if (s < 131072) return 59999;
    if (s < 524288) return 100019;
    if (s < 2097152) return 300007;
    if (s < 8388608) return 900001;
    if (s < 33554432) return 1000003;
    if (s < 134217728) return 3000017;
    if (s < 536870912) return 5000011;
    return 7000003;
}
