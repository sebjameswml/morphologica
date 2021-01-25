#include "collision.h"

using namespace morph::softmats;

// Collision List
CollisionList::CollisionList(){

}

void CollisionList::push( Collision* c ){
    vector<Collision *>::iterator it;
    // cout << "Number of collisions before: " << collisions.size() << endl;
    // if( collisions.empty() )collisions.push_back( c );
    bool exist = false;
    if( c->ctype == 0 ){
        FPCollision *fpc = (FPCollision*)c;

        for( Collision *g : collisions ){
            if( g->ctype == 0 )
                if( ((FPCollision*)g)->f == fpc->f && ((FPCollision*)g)->p == fpc->p )
                    exist = true;
        }
    }

    if( !exist )
        collisions.push_back( c );
    // else{
    //     for( it = collisions.begin(); it != collisions.end(); ++it  ){
    //         if( (*it)->hc > c->hc ){
    //             collisions.insert( it, c );
    //             break;
    //         }
    //     }
    // }

    // cout << "collision list: ";
    // for( it = collisions.begin(); it != collisions.end(); ++it  )
    //     cout << (*it)->hc << ", ";
    // cout << endl;

    // cout << "Number of collisions after: " << collisions.size() << endl;
    // cin.get();
}

void CollisionList::clear(){
    collisions.clear();
}

int CollisionList::count(){
    return collisions.size();
}

bool CollisionList::isEmpty(){
    return collisions.empty();
}

Collision* CollisionList::pop(){
    // cout << "Poping!" << collisions.size() << endl;
    if( !this->isEmpty() ){
        // cout << "Getting collision" << endl;
        Collision *c = this->collisions[0];
        this->collisions.erase( collisions.begin() );

        return c;
    }

    return NULL;
}

void CollisionList::discount( double hc ){
    for( Collision *c : collisions )
        c->hc -= hc;
}

// Collision
Collision::Collision( double hc, vec cp, vec cnormal ):hc(hc), cp(cp), cnormal(cnormal), active(true){

}

// FPCollision
FPCollision::FPCollision( double hc, vec cp, vec cnormal, Face* f, Point* p ):Collision(hc, cp, cnormal),f(f),p(p){
    this->ctype = 0;
}

void FPCollision::solve(CollisionTest *collisionTest){

    Collision* c = collisionTest->testFPCollision( this->f, this->p );

    if( c != nullptr ){
        this->cp = c->cp;
        this->hc = c->hc;
        
        double cr = 1.0;
        double cc = arma::dot(p->x_c - cp, f->normal_c);

        if( cc < 0 ){
            if( p->w > 0 && !p->lock )
                p->x_c += -cc*cr*f->normal_c;
            // p->x_c = cp;

            for( Point *q : f->points){
                if( q->w > 0 && !q->lock ){
                    q->x_c -= -cc*cr*f->normal_c;
                }
            }
        }

        delete c;
    }
	
}

void FPCollision::updateVelocity(){
    arma::vec vel, fvec;
	double cr = 0.01;
    vel = p->v;
    vel -= 2.0 * cr * arma::dot(vel, f->normal_c) * f->normal_c;
    fvec = -(vel - arma::dot(vel, f->normal_c) * f->normal_c);
    
    if( p->w > 0 && !p->lock )
        p->v += fvec;

    for( Point *q : f->points){
        if( q->w > 0 && !q->lock )
            q->v -= fvec;
    }
}

// EECollision
EECollision::EECollision( double hc, vec cp, vec cnormal, Edge e1, Edge e2 ):Collision(hc,cp,cnormal), e1(e1), e2(e2){
     this->ctype = 1;
}
    
    
void EECollision::solve(CollisionTest *collisionTest){
    // change velocities - inelastic collision
    
    Collision* c = collisionTest->testEECollision( this->e1, this->e2 );

    if( c != nullptr ){
        // updating collision
        this->cp = c->cp;
        this->hc = c->hc;
        this->cnormal = c->cnormal;

        Point *p1 = e1.p1;
        Point *p2 = e1.p2;
        Point *q1 = e2.p1;
        Point *q2 = e2.p2;
        double cr = 1.0;
        double cc1 = arma::dot(p1->x_c - cp, cnormal);
        double cc2 = arma::dot(p2->x_c - cp, cnormal);
        double cc3 = arma::dot(q1->x_c - cp, cnormal);
        double cc4 = arma::dot(q2->x_c - cp, cnormal);
        if( p1->w > 0 && !p1->lock )
            p1->x_c += -cc1*cr*cnormal;
        if( p2->w > 0 && !p2->lock )
            p2->x_c += -cc2*cr*cnormal;
        if( q1->w > 0 && !q1->lock )
            q1->x_c -= -cc3*cr*cnormal;
        if( q2->w > 0 && !q2->lock )
            q2->x_c -= -cc1*cr*cnormal;
    }
}

void EECollision::updateVelocity(){
    Point *p1 = e1.p1;
    Point *p2 = e1.p2;
    Point *q1 = e2.p1;
    Point *q2 = e2.p2;

    arma::vec vel, fvec;
	double cr = 1.0;
    
    if( p1->w > 0 && !p1->lock ){
        vel = p1->v;
        vel -= 2.0 * cr * arma::dot(vel, cnormal)*cnormal;
        fvec = -(vel - arma::dot(vel, cnormal)*cnormal);    
        p1->v += fvec;
    }

    if( p2->w > 0 && !p2->lock ){
        vel = p2->v;
        vel -= 2.0 * cr * arma::dot(vel, cnormal)*cnormal;
        fvec = -(vel - arma::dot(vel, cnormal)*cnormal);    
        p2->v += fvec;
    }

    if( q1->w > 0 && !q1->lock ){
        vel = q1->v;
        vel -= 2.0 * cr * arma::dot(vel, cnormal)*cnormal;
        fvec = -(vel - arma::dot(vel, cnormal)*cnormal);    
        q1->v += fvec;
    }

    if( q2->w > 0 && !q2->lock ){
        vel = q2->v;
        vel -= 2.0 * cr * arma::dot(vel, cnormal)*cnormal;
        fvec = -(vel - arma::dot(vel, cnormal)*cnormal);    
        q2->v += fvec;
    }
    
}