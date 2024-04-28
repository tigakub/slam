#ifndef __AVERAGE_H__
#define __AVERAGE_H__

class Average {
    protected:
        int periods;
        double *buffer;
        int index;
        int count;

    public:
        Average(int iPeriods): periods(iPeriods), buffer(new double[periods]), index(0), count(0) {
            for(int i = 0; i < periods; i++) {
                buffer[i] = 0.0;
            }
        }
        virtual ~Average() {
            if(buffer) delete [] buffer;
        }

        double operator()(double iCurrent) {
            count < periods ? ++count : periods;
            buffer[index] = iCurrent;
            double sum = 0.0;
            for(int i = 0; i < periods; i++) {
                sum += buffer[i];
            }
            index = (index + 1) % periods;
            return sum / double(count);
        }
};

#endif
