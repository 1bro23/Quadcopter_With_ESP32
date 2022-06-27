//how to use
//MyProcessing forData1;
//float data = forData2.smoothLikeButter(cData, 20);
//!profit!//

#include <queue>

#ifndef MYPROCESSING_HPP

std::queue<float> myq;

class MyProcessing{
    public:
        int i;
        float cData,total;

        float smoothLikeButter(float data,int nSample = 10){
            i++;

            this->cData = data;
            this->total += this->cData;
            myq.push(this->cData);

            if(i>nSample){
                i=nSample;
                total -= myq.front();
                myq.pop();
            }

            return total/i;
        }
};

#endif