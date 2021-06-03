#include "processtrackingdata.h"
using namespace std;


//构造函数
processTrackingData::processTrackingData(string str)
{
    this->data = str;
    evaluateData();
    if(dataIsValid)
    {
        getProbeCoordinate();
        getProbeAttitude();
        getRigidCoordinate();
    }
}

//判断ndi返回的数据是否有效
void processTrackingData::evaluateData()
{
    int index1 = data.find("MISSING");
    int index2 = data.find("DISABLE");
    int dataLength = data.size();
    if ((index1 == -1) & (index2 == -1) & (dataLength==320)){
        dataIsValid = true;
        return;
    }
    else
    {
        dataIsValid = false;
        return;
    }
}

void processTrackingData::getProbeCoordinate()
{
    string x = string(data,28,7);
    probeTx = atoi(x.data())/100.0;

    string y = string(data,35,7);
    probeTy = atoi(y.data())/100.0;

    string z = string(data,42,7);
    probeTz = atoi(z.data())/100.0;

}

void processTrackingData::getProbeAttitude()
{
    string Q0 =string(data,4,6);
    string Qx =string(data,10,6);
    string Qy =string(data,16,6);
    string Qz =string(data,22,6);

    probeQ0 = atoi(Q0.data())/10000.0;
    probeQx = atoi(Qx.data())/10000.0;
    probeQy = atoi(Qy.data())/10000.0;
    probeQz = atoi(Qz.data())/10000.0;
}

void processTrackingData::getRigidCoordinate()
{
    string x1 = string(data,231,7);
    string y1 = string(data,238,7);
    string z1 = string(data,245,7);
    rigidx1 = atoi(x1.data())/100.0;rigidy1 = atoi(y1.data())/100.0;rigidz1 = atoi(z1.data())/100.0;

    string x2 = string(data,252,7);
    string y2 = string(data,259,7);
    string z2 = string(data,266,7);
    rigidx2 = atoi(x2.data())/100.0;rigidy2 = atoi(y2.data())/100.0;rigidz2 = atoi(z2.data())/100.0;

    string x3 = string(data,273,7);
    string y3 = string(data,280,7);
    string z3 = string(data,287,7);
    rigidx3 = atoi(x3.data())/100.0;rigidy3 = atoi(y3.data())/100.0;rigidz3 = atoi(z3.data())/100.0;

    string x4 = string(data,294,7);
    string y4 = string(data,301,7);
    string z4 = string(data,308,7);
    rigidx4 = atoi(x4.data())/100.0;rigidy4 = atoi(y4.data())/100.0;rigidz4 = atoi(z4.data())/100.0;

}
