#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "entt/src/entt/entt.hpp"

#define DIMX 640
#define DIMY 480
#define POPULATION 10000
#define MALATI_INIZIALI 20
#define PROB_AMMALARSI 20
#define PROB_MORTE 3
#define TEMPO_MALATTIA 20

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

struct ill {
    float tempo = TEMPO_MALATTIA;
};
struct healty {
};

struct recovered {
};

void initPopulation(entt::registry& reg) {
    for(auto i = 0u; i < POPULATION; ++i) {
        const auto entity = reg.create();
        reg.emplace<position>(entity, float(std::rand() % DIMX), float(std::rand() % DIMY));
        reg.emplace<velocity>(entity, float((std::rand() % 50)/100.0 - 0.245), float((std::rand() % 50)/100.0 - 0.245));
        if (std::rand() % 1000 < MALATI_INIZIALI) {
            reg.emplace<ill>(entity);
        }
        else {
            reg.emplace<healty>(entity);
        }
    }
}

void updatePosition(entt::registry& reg) {
    // TODO don't move death
    auto view = reg.view<position, velocity>();
    for (const entt::entity e : view) {
        auto & p =view.get<position>(e); 
        auto & v =view.get<velocity>(e); 
        p.x += v.dx;
        p.y += v.dy;

        // collision with borders
        if(p.x <= 0) { p.x = 0; v.dx *=-1;}
        if(p.x >= DIMX) { p.x = DIMX; v.dx *=-1;}
            
        if(p.y <= 0) { p.y = 0; v.dy *=-1;}
        if(p.y >= DIMY) { p.y = DIMY; v.dy *=-1;}
    }
}

void updateHealth(entt::registry& reg, sf::Time elapsed) {
    auto view = reg.view<ill>();
    for (const entt::entity e : view) {
        auto & p =view.get<ill>(e);
        p.tempo -= elapsed.asSeconds();
        if(p.tempo < 0) {
            reg.remove<ill>(e);
            if((std::rand() % 100) >= PROB_MORTE) { 
                reg.emplace<recovered>(e);
            }
        }
    }
}

void calcCollision(entt::registry& reg) {
    auto view = reg.view<position, velocity>();
    

    for (auto it = view.begin(); it!= view.end(); ++it) {
        auto& en1 = *it;
        auto & p1 =view.get<position>(en1); 
        auto & v1 =view.get<velocity>(en1); 
        auto it2 = it;
        ++it2;
        for (; it2!= view.end(); ++it2) {
            auto& en2 = *it2;
            auto & p2 =view.get<position>(en2); 
            auto & v2 =view.get<velocity>(en2); 
            if(std::abs(p1.x - p2.x) < 2 && std::abs(p1.y - p2.y) < 2) {
                //std::cout<<"COLLISION"<<std::endl;
                v1.dx = (std::rand() % 50)/100.0 - 0.245;
                v1.dy = (std::rand() % 50)/100.0 - 0.245;
                v2.dx = (std::rand() % 50)/100.0 - 0.245;
                v2.dy = (std::rand() % 50)/100.0 - 0.245;

                if (reg.has<ill>(en1) && reg.has<healty>(en2)) {
                    if((std::rand() % 100) < PROB_AMMALARSI) {
                        reg.remove<healty>(en2);
                        reg.emplace<ill>(en2);
                    }
                }

                if (reg.has<ill>(en2) && reg.has<healty>(en1)) {
                    if((std::rand() % 100) < PROB_AMMALARSI) {
                        reg.remove<healty>(en1);
                        reg.emplace<ill>(en1);
                    }
                }
            }
        }
    }
}

void update(entt::registry& reg, sf::Time elapsed) {
    updatePosition(reg);
    calcCollision(reg);
    updateHealth(reg, elapsed);
    std::cout<<"sani:"<<reg.view<healty>().size()<< "\tmalati:"<<reg.view<ill>().size()<<"\tmorti:"<<(reg.size() - reg.view<healty>().size() - reg.view<ill>().size() - reg.view<recovered>().size())<<"\tguariti:"<<reg.view<recovered>().size()<<std::endl;
}

void draw(sf::RenderWindow & w, entt::registry& reg) {
    sf::VertexArray va;

    for (auto view = reg.view<position, healty>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(255u,255u,255u)));   
    }
    for (auto view = reg.view<position, ill>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(255u,0,0)));   
    }
    for (auto view = reg.view<position, recovered>(); const entt::entity e : view) {
        auto & p =view.get<position>(e);
        va.append(sf::Vertex(sf::Vector2f(p.x, p.y), sf::Color(0,0,255)));   
    }
    w.draw(va);
}



/*
*enum Status {
*    sano,
*    malato,
*    morto,
*    guarito,
    vaccinato,
*};
*
*struct entity {
*    float x;
*    float y;
*    float dx;
*    float dy;
*    Status status;
*    float tempo;
*    
*    
*    entity() {
*        x = std::rand() % 640;
*        y = std::rand() % 480;
*        dx = (std::rand() % 50)/100.0 - 0.245;
*        dy = (std::rand() % 50)/100.0 - 0.245;
*        if(std::rand() % 1000 < MALATI_INIZIALI){
*            status =  malato;
*            tempo = TEMPO_MALATTIA;
*        } else {
*            status =  sano;
*            tempo = 0;
*        }
*        
*        
*        //std::cout<<"x:"<<x<<" y:"<<y<<std::endl;
*    }
*};
*
*
*void update(std::vector<entity>& ent, sf::VertexArray &va, sf::Time elapsed) {
*    
*    
*    //movement
*    for (auto & v: ent) {
*        if(v.status != morto) {
*            // update position
*            v.x += v.dx;
*            v.y += v.dy;
*            
*            // collision with borders
*            if(v.x <= 0) { v.x = 0; v.dx *=-1;}
*            if(v.x >= 640) { v.x = 640; v.dx *=-1;}
*            
*            if(v.y <= 0) { v.y = 0; v.dy *=-1;}
*            if(v.y >= 480) { v.y = 480; v.dy *=-1;}
*        
*        }
*    }
*    
*    //collision detection
*    unsigned int i = 0;
*    for (auto & v: ent) {
*        ++i;
*        //std::cout <<"------------"<<std::endl;
*        if(v.status != morto) {
*            size_t s = ent.size();
*            for(unsigned int n = i; n < s; ++n) {
*                if(ent[n].status != morto) {
*                    //std::cout <<"test"<<std::endl;
*                    if(std::abs(v.x - ent[n].x) < 2 && std::abs(v.y - ent[n].y) < 2) {
*                        //if(i == 1) std::cout<<"COLLISION between "<<i<<" and "<<n+1<<" elements" <<std::endl;
*                        v.dx = (std::rand() % 50)/100.0 - 0.245;
*                        v.dy = (std::rand() % 50)/100.0 - 0.245;
*                        ent[n].dx = (std::rand() % 50)/100.0 - 0.245;
*                        ent[n].dy = (std::rand() % 50)/100.0 - 0.245;  
*                        if (v.status == malato && ent[n].status == sano) {
*                            if((std::rand() % 100) < PROB_AMMALARSI) {
*                                ent[n].status = malato;
*                                ent[n].tempo = TEMPO_MALATTIA;
*                            }
*                        }
*                        if (ent[n].status == malato && v.status == sano) {
*                            if((std::rand() % 100) < PROB_AMMALARSI) {
*                                v.status = malato;
*                                v.tempo = TEMPO_MALATTIA;
*                            }
*                        }
*                    }
*                }
*            }
*        }
*    }
    
    //update malattia
    for (auto & v: ent) {
        if(v.status == malato) {
            v.tempo -= elapsed.asSeconds();
            if(v.tempo < 0) {
                if((std::rand() % 100) < PROB_MORTE) {
                    v.status = morto;
                    v.x = -10;
                    v.y = -10;
                } else {
                    v.status = guarito;
                }
            }
        }
    }
    
    i = 0;
    unsigned int sani = 0;
    unsigned int malati = 0;
    unsigned int morti = 0;
    unsigned int guariti = 0;
    unsigned int vaccinati = 0;
    for(auto & v: ent){
        
        switch(v.status) {
            case sano:
                va[i].color = sf::Color(255, 255, 255);
                ++sani;
                break;
            case malato:
                va[i].color = sf::Color(255, 0, 0);
                ++malati;
                break;
            case morto:
                va[i].color = sf::Color(0, 0, 0);
                ++morti;
                break;
            case guarito:
                va[i].color = sf::Color(0, 0, 255);
                ++guariti;
                break;
            case vaccinato:
                va[i].color = sf::Color(0, 255, 255);
                ++vaccinati;
                break;
            default:
                std::cout<<"ERRRORE"<<std::endl;
                va[i].color = sf::Color(255, 255, 255);
        }
        ++i;

    }
    
    i = 0;
    for(auto & v: ent){
        va[i++].position = sf::Vector2f(v.x, v.y);
    }
    std::cout<<"sani:"<<sani<< "\tmalati:"<<malati<<"\tmorti:"<<morti<<"\tguariti:"<<guariti<<"\tvaccinati:"<<vaccinati<<std::endl;
    
}*/

int main()
{

    entt::registry registry;

    initPopulation(registry);
    // create the window
    sf::RenderWindow window(sf::VideoMode(640, 480), "Particles");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(50);

    // create the parti60000cle system
    //ParticleSystem particles(200);

    // create a clock to track the elapsed time
    sf::Clock clock;
    sf::Clock occupancyClock;
    //int count = 0;

    // run the main loop
    while (window.isOpen())
    {
        occupancyClock.restart();
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        sf::Time evtTime = occupancyClock.restart();
        // update it
        sf::Time elapsed = clock.restart();
        update(registry, elapsed);
        
        sf::Time simTime = occupancyClock.restart();

        // draw it
        window.clear();
        draw(window, registry);
        sf::Time drawTime = occupancyClock.restart();
        window.display();
        sf::Time displayTime = occupancyClock.restart();
        
        //std::cout<<"evt:"<<evtTime.asMicroseconds()<<" sim:"<<simTime.asMicroseconds()<<" draw:"<<drawTime.asMicroseconds()<<" display:"<<displayTime.asMicroseconds()<<std::endl;
    }

    return 0;
}
 
