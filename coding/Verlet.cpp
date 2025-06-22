#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>

using namespace std;

#define G 1 //Approximation
#define dt 0.01
#define pi 3.14159265358979323846

class Body{
    public:
    float mass, radius, theta; // theta in radian
    sf::Vector2f position,oldposition,velocity;
    Body(float mass, float radius, float x, float y, float vel, float theta){
        this->mass=mass;
        this->radius=radius;
        position.x=x;
        position.y=y;
        oldposition.x=x;
        oldposition.y=y;
        //theta=theta*(pi/180.0);
        velocity.x=vel*cos(theta);
        velocity.y=vel*sin(theta);
    }

    sf::CircleShape getCircle(){
        sf::CircleShape Circle(radius);
        Circle.setPosition({position.x, position.y});
        Circle.setFillColor(sf::Color::Green);
        return Circle;
    }

};

float distance(const Body& b1,const Body& b2){
    float dx=b2.position.x-b1.position.x;
    float dy=b2.position.y-b1.position.y;

    //finding distance
    float distance=sqrt((dx*dx)+(dy*dy));

    return distance;
}

sf::Vector2f acceleration(const Body& b1,const Body& b2){
    //Direction vector
    float dx=b2.position.x-b1.position.x;
    float dy=b2.position.y-b1.position.y;

    //finding distance
    float distance=sqrt((dx*dx)+(dy*dy));

    //unit vectors for direction
    float ux=dx/distance;
    float uy=dy/distance;

    //calculating force
    float F=G*b1.mass*b2.mass/(distance*distance);

    //calculating acceleration
    sf::Vector2f AccVec;
    AccVec.x=(F/b1.mass)*ux;
    AccVec.y=(F/b1.mass)*uy;

    return AccVec;
}

int main()
{   
    ifstream infile("input.txt");
    if (!infile) {
        cerr << "Could not open input file.\n";
        return 1;
    }

    //No of bodies
    int n;
    //cout<<"Enter no of bodies\n";
    infile>>n;
    
    //creating vector of bodies
    vector<Body>bodies;
    for(int i=0;i<n;i++){
        float mass, radius, x, y, velocity, theta;
        //cout<<"Enter mass, radius, x coordinate, y coordinate, velocity and theta all in SI Units\n";
        infile>>mass>>radius>>x>>y>>velocity>>theta;
        bodies.push_back(Body(mass, radius, x, y, velocity, theta));
    }
    
    //vector of circles
    vector<sf::CircleShape>circles;
    for(int i=0;i<n;i++){
        sf::CircleShape circle=bodies[i].getCircle();
        circles.push_back(circle);
    }
    
    //trails for displaying orbits
    vector<vector<sf::Vertex>> trails(n); // One trail per body

    //2d acceleration matrix
    vector<vector<sf::Vector2f>>accMatrix(n,vector<sf::Vector2f>(n));

    //physics loop to get 1st iteration of position vector
        for(int i=0;i<n;i++){                                 //acc update
            for(int j=i;j<n;j++){
                sf::Vector2f a;
                if(i==j){
                    a={0,0};
                }
                else{
                    a=acceleration(bodies[i],bodies[j]);
                }
                accMatrix[i][j]=a;
                accMatrix[j][i]=-a;
            }
        }

        for(int i=0;i<n;i++){                       //velocity update
            for(int j=0;j<n;j++){
                bodies[i].velocity.x+=accMatrix[i][j].x*dt;
                bodies[i].velocity.y+=accMatrix[i][j].y*dt;
            }
            bodies[i].position.x+=bodies[i].velocity.x*dt;      //position update
            bodies[i].position.y+=bodies[i].velocity.y*dt;

            circles[i].setPosition({bodies[i].position});
        }
        
    //window
    sf::RenderWindow window;
    window.create(sf::VideoMode({500,500}),"SFML");
    window.setFramerateLimit(60);

    while(window.isOpen()){
        while(optional event=window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }
        //physics loop
        for(int i=0;i<n;i++){                   //acc update
            for(int j=i;j<n;j++){
                sf::Vector2f a;
                if(i==j){
                    a={0,0};
                }
                else{
                    a=acceleration(bodies[i],bodies[j]);
                }
                accMatrix[i][j]=a;
                accMatrix[j][i]=-a;
            }
        }

        for(int i=0;i<n;i++){          
            sf::Vector2f temp=bodies[i].position;   //to store old position              
            sf::Vector2f acc={0.0f,0.0f};
            for(int j=0;j<n;j++){                   //total acceleration on a body
                acc.x+=accMatrix[i][j].x;
                acc.y+=accMatrix[i][j].y;
            }
            //Uses Verlet formula: x(n+1)=2*x(n)-x(n-1)+a(n)*dt^2
            bodies[i].position.x=(2*bodies[i].position.x)-bodies[i].oldposition.x+acc.x*dt*dt;  
            bodies[i].position.y=(2*bodies[i].position.y)-bodies[i].oldposition.y+acc.y*dt*dt;

            bodies[i].oldposition=temp;      //old position update

            circles[i].setPosition({bodies[i].position});

            //Trails
            sf::Vertex vertex={bodies[i].position+sf::Vector2f{bodies[i].radius,bodies[i].radius},sf::Color::White};
            trails[i].push_back(vertex);
            //to avoid memory overflow
             int MaxLength=5000;
             if(trails[i].size()>MaxLength){
                 trails[i].erase(trails[i].begin());
             }
        }

        //rendering
        window.clear();
        for(int i=0;i<n;i++){
            if (trails[i].size() > 1)
            window.draw(&trails[i][0], trails[i].size(), sf::PrimitiveType::LineStrip);
            window.draw(circles[i]);
        }
        window.display();
    }
    return 0;
}