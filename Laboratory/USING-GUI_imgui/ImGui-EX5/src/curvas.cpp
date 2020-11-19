#include "curvas.h"

void addPointsCurveG2(
    std::vector<double> &vert, 
    glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, 
    int n_points){
        
    float inc = 1.0/(n_points-1);
    for(float t = 0.0f; t <= 1; t += inc){
        
        glm::vec2 p =   p0*(1.f-t)*(1.f-t) + 
                        p1*2.f*t*(1.f-t) + 
                        p2*t*t;

        vert.push_back(p.x);
        vert.push_back(p.y);
    }
}

void addPointsPelo(
    std::vector<double> &vert, 
    glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, 
    int n_points,
    double tam_pelo){
    int c = 1;
    float inc = 1.0/(n_points-1);
    for(float t = 0.0f; t <= 1; t += inc){
        
        glm::vec2 p =   p0*(1.f-t)*(1.f-t) + 
                        p1*2.f*t*(1.f-t) + 
                        p2*t*t;
        if (c % 2 == 0){
            glm::vec2 aux = glm::vec2(vert[vert.size()-2], vert[vert.size()-1]);
            aux = p - aux;
            aux = glm::vec2(-1*aux.y, aux.x);
            // aux = aux / (float)sqrt(aux.x * aux.x + aux.y + aux.y);
            p = p + aux*(float)tam_pelo/2.f;

        }
        if (c!=1 && (c+1) % 2 == 0){
            vert.push_back(p.x);
            vert.push_back(p.y);
        }
        vert.push_back(p.x);
        vert.push_back(p.y);
        c++;
    }
}

void addPointsCurveG3(
    std::vector<double> &vert, 
    glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, 
    int n_points){

    float inc = 1.0/(n_points-1);
    for(float t = 0.0f; t <= 1; t += inc){
       
       glm::vec2 p =    p0*(1.f-t)*(1.f-t)*(1.f-t) + 
                        p1*3.f*t*(1.f-t)*(1.f-t) + 
                        p2*3.f*t*t*(1.f-t) + 
                        p3*t*t*t;

       vert.push_back(p.x);
       vert.push_back(p.y);
   }
}

void addPointsCurveG4(
    std::vector<double> &vert, 
    glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, 
    int n_points){

    float inc = 1.0/(n_points-1);
    for(float t = 0.0f; t <= 1; t += inc){

        glm::vec2 p =   p0*(1.f-t)*(1.f-t)*(1.f-t)*(1.f-t) +
                        p1*3.f*t*(1.f-t)*(1.f-t)*(1.f-t) + 
                        p2*6.f*t*t*(1.f-t)*(1.f-t) + 
                        p3*t*t*t*(1.f-t)+
                        p4*t*t*t*t;
        
        vert.push_back(p.x);
        vert.push_back(p.y);
    }
}

void addPointCircle(std::vector<double> &vert, double r, glm::vec2 center, int n_points){
    float inc = (2*3.14)/n_points;
    for(float t = -2*3.14; t <= 3.14; t += inc){
        
        glm::vec2 p = glm::vec2(center.x + r * sin(t), center.y + r * cos(t));
        
        vert.push_back(p.x);
        vert.push_back(p.y);
    }
}
