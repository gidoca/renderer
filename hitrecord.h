#ifndef HITRECORD_H
#define HITRECORD_H

class HitRecord
{

  public:
    HitRecord(double rayParameter);
    static HitRecord noIntersection();
    double getRayParameter() const;
    bool getIntersects() const;
    
  private:
    double rayParameter;
    bool intersects;
    HitRecord() : rayParameter(0) {};

};

#endif // HITRECORD_H
