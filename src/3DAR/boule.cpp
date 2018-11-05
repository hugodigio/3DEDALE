//  Created by HASSANEIN Alzahra

#include "headers/boule.hpp"

/*
 FAIT Voir si le soucis de couleur de la boule est regle, par defaut la couleur verte
 du dernier objet dessine
 
 FAIRE Definir la vitesse a partir de l'angle de Rotation du plateau
 FAIRE un deplacecment plus naturel :  en fonction de la circonference
 
 FAIRE un tableau de ligne pour dessiner les murs internes
 Utiliser ce tableau de lignes pour checker les collisions
 
 Voir la latence du timer
 Mouvement diagonale

 Commenter
 
 */

    
void remplissageCoordMur(){
//    //Murs
//    murCoordX.push_back(-LONGUEUR*1./2.);
//    murCoordY.push_back(SECTION*1./2.);
//
//    murCoordX.push_back(LONGUEUR*1./2.);
//    murCoordY.push_back(SECTION*1./2.);
//
//    murCoordX.push_back(LONGUEUR*1./2.);
//    murCoordY.push_back(-SECTION*1./2.);
//
//    murCoordX.push_back(-LONGUEUR*1./2.);
//    murCoordY.push_back(-SECTION*1./2.);
//


//    Xmax = *max_element(murCoordX.begin(), murCoordX.end());
////    cout<<"XMax value: "<<Xmax<<endl;
//    Ymax = *max_element(murCoordY.begin(), murCoordY.end());
////    cout<<"XMax value: "<<Ymax<<endl;
//    Xmin = *min_element(murCoordX.begin(), murCoordX.end());
////    cout<<"XMin value: "<<Xmin<<endl;
//    Ymin = *min_element(murCoordY.begin(), murCoordY.end());
////    cout<<"XMin value: "<<Ymin<<endl;
    
  
//    bouleX =-0.3-SECTION/2.;
//    bouleY =-0.3+SECTION/2.;
    
    A.push_back(Point2f(-LONGUEUR*1./2.,SECTION*1./2.));
    B.push_back(Point2f(LONGUEUR*1./2.,SECTION*1./2.));
    
    A.push_back(Point2f(LONGUEUR*1./2.,SECTION*1./2.));
    B.push_back(Point2f(LONGUEUR*1./2.,-SECTION*1./2.));
    
    A.push_back(Point2f(LONGUEUR*1./2.,-SECTION*1./2.));
    B.push_back(Point2f(-LONGUEUR*1./2.,-SECTION*1./2.));
    
    A.push_back(Point2f(-LONGUEUR*1./2.,-SECTION*1./2.));
    B.push_back(Point2f(-LONGUEUR*1./2.,SECTION*1./2.));
    
    cout << "A" <<endl;
    affichePVector(&A);
    cout << "B" <<endl;
    affichePVector(&B);
    
    initEquDroites();

}


void initEquDroites (){
    //creation d'un vecteur des equations des droites, stock
    // verifie si P.y = coeffDirecteur * P.x + k
    
    float tmpCoeff, tmpConstK;
    vector<Point2f> tmpA = vectPointcopy(A);
    vector<Point2f> tmpB = vectPointcopy(B);
    
    //    cout << "tmpA taille " << tmpA.size() << endl;
    //    affichePVector(&tmpA);
    //    cout << "tmpB" << tmpB.size() <<  endl;
    //    affichePVector(&tmpB);
    
    
    for (int i=0; i<A.size(); i++){
        Point2f ptDep = tmpA.back();
        tmpA.pop_back();
        
        //cout << "tmpA" << endl;
        cout << "Tour : " << i << " DEPART " << ptDep << endl;
        //affichePVector(&tmpA);
        
        Point2f ptArr = tmpB.back();
        tmpB.pop_back();
        
        //cout << "tmpB" << endl;
        cout << "Tour : " << i << " ARRIVEE " << ptArr << endl <<endl;;
        //affichePVector(&tmpB);
        
        if((ptArr.x-ptDep.x) == 0){
            tmpCoeff = 9999;
            tmpConstK = (ptDep.x);
        }else{
            tmpCoeff = ((ptArr.y-ptDep.y)/(ptArr.x-ptDep.x));
            tmpConstK = (ptDep.y)-(tmpCoeff*(ptDep.x));
        }
        coeffDirecteur.push_back(tmpCoeff);
        constK.push_back(tmpConstK);
    }
    cout << "CoeffDirecteur" <<endl;
    afficheFVector(&coeffDirecteur);
    cout << "ConstK" <<endl;
    afficheFVector(&constK);
}

/**************************************************************************************************/

/***********************************************
 ***************   Affichages   ****************
 ***********************************************
 ***********************************************/


void affichePVector( vector<Point2f> * v){
    for (int i=0; i<v->size(); i++)
        cout << v->at(i) << endl;
}


void afficheFVector( vector<float> * v){
    for (int i=0; i<v->size(); i++)
        cout << v->at(i) << endl;
}


/**************************************************************************************************/

/***********************************************
 ****************     Copie     ****************
 ***********************************************
 ***********************************************/


vector<Point2f> vectPointcopy(vector<Point2f> vect){
    
    vector <Point2f> newVector;
    for (int i=0; i<vect.size(); i++)
        newVector.push_back(vect[i]);
    
    return newVector;
}

vector<float> vectCopy(vector<float> vect){
    
    vector <float> newVector;
    for (int i=0; i<vect.size(); i++)
        newVector.push_back(vect[i]);
    
    return newVector;
}

/**************************************************************************************************/

/***********************************************
 ************    Plateau et Murs    ************
 ***********************************************
 ***********************************************/

void def_murfictif(){
    // face rouge, de droite
    //rotation de 90 selon l'axe des y et translation sur les x et z
    glPushMatrix();
        glColor3f(1.0, 0.0, 1.0);
        glTranslatef(LONGUEUR/3.0, 0.0, 0.0);
        //glRotatef(90, 0, 1, 0);
        //glScalef(SECTION/3.,SECTION,SECTION);
        //glScalef(SECTION,SECTION,SECTION/2.0);
        //glScalef(LONGUEUR*a,SECTION,SECTION);
    
        glBegin(GL_POLYGON);
            glVertex3f( 0.0 ,-1/2., 0.0);
            glVertex3f( 0.0  ,1/2., 0.0);
            glVertex3f( 0.0 ,1/2., -SECTION/4.);
            glVertex3f( 0.0 ,-1/2., -SECTION/4.);
        glEnd();
    glPopMatrix();
}

//Trace les axes, X rouge, Y Vert, Z Bleu
void def_axes(void){

    glBegin(GL_LINES);
    //x
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(-1.,0.0,0.0);
        glVertex3f(1.,0.0,0.0);
        //y
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0,-1,0.0);
        glVertex3f(0.0,1.,0.0);
        //z
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0,0.0,-1.);
        glVertex3f(0.0,0.0,1.);
    glEnd();

}


void def_carre(void){
    int a = 1;
    glBegin(GL_POLYGON);
        glVertex2f(-a/2.0,a/2.0);
        glVertex2f(a/2.0,a/2.0);
        glVertex2f(a/2.0,-a/2.0);
        glVertex2f(-a/2.0 ,-a/2.0);
    glEnd();


}

void def_boule(void){
    
    glPushMatrix();
        //float blanc[] = { 1.0F,1.0F,1.0F,1.0F };
        //glMaterialfv(GL_FRONT,GL_SPECULAR,blanc);
        glColor3f(0.5, 0.5, 0.0);
        glutSolidSphere(0.3,36,36);
    glPopMatrix();
    
}

void def_plateau(void){
    
    glPushMatrix();
        glColor3f(0.0, 0.5, 1.0);
        glTranslatef(0.0, 0.0, -SECTION/4.0);
        glRotatef(180, 1, 0, 0);
        glScalef(SECTION*3,SECTION*2.,SECTION);
        def_carre();
    glPopMatrix();
}


void def_boite(void){

    // face bleu, le fond
    //on le met devant en translatant selon z
    glPushMatrix();
        glColor3f(0.0, 0.1, 1.0);
        glTranslatef(0.0, 0.0, -SECTION/4.0);
        glRotatef(180, 1, 0, 0);
        glScalef(LONGUEUR,SECTION,SECTION);
        def_carre();
    glPopMatrix();

    // face rouge, de droite
    //rotation de 90 selon l'axe des y et translation sur les x et z
    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(LONGUEUR/2.0, 0.0, 0.0);
        glRotatef(90, 0, 1, 0);
        glScalef(SECTION/2.,SECTION,SECTION);
        //glScalef(SECTION,SECTION,SECTION/2.0);
        //glScalef(LONGUEUR*a,SECTION,SECTION);
        def_carre();
    glPopMatrix();

    // face jaune, de gauche
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glTranslatef(-LONGUEUR/2.0, 0.0,0.0);
        glRotatef(-90, 0, 1, 0);
        glScalef(SECTION/2.,SECTION,SECTION);
        //glScalef(SECTION,LONGUEUR*a,SECTION);
        //glScalef(LONGUEUR*a,SECTION,SECTION);
        def_carre();
    glPopMatrix();

    // face blanche, de haut

    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(0.0, SECTION/2.0, 0.0);
        glRotatef(-90, 1, 0, 0);
        glScalef(LONGUEUR,SECTION/2.,SECTION);
        //glScalef(SECTION,LONGUEUR*a,SECTION);
        def_carre();
    glPopMatrix();

    //face de bas, verte
    glPushMatrix();
        glColor3f(0.0, 1.0, 0.0);
        glTranslatef(0.0, -SECTION/2.0, 0.0);
        glRotatef(90, 1, 0, 0);
        glScalef(LONGUEUR,SECTION/2.,SECTION);
        //glScalef(SECTION,LONGUEUR*a,SECTION);
        def_carre();
    glPopMatrix();

}

/**************************************************************************************************/


/***********************************************
 *********    Rotations Plateaux    ************
 ***********  Mouvement de la boule  ***********
 ***********************************************/


//Axes : X rouge, Y Vert, - X, | Y
void RotationM(float M[16], int valaxe, float angle) {
    glPushMatrix();
    glLoadIdentity();
    glLoadMatrixf(M);
    
    switch (valaxe) {
        case 1: //x
            glRotatef(angle, 1.0, 0.0, 0.0);
            rotationActuelleX += angle;
            //cout << "ROTATION SUR XXXXXXXXXXXXXXXXX " << angle << " X " <<rotationActuelleX << endl;
            break;
        case 2: //y
            glRotatef(angle, 0.0, 1.0, 0.0);
            rotationActuelleY += angle;
            //cout <<"ROTATION SUR YYYYYYYYYYYYYYYYYY " <<  angle << " Y "<<   rotationActuelleY << endl;
            break;
            //        case 3://z
            //            glRotatef(angle, 0.0, 0.0, 1.0);
            //            break;
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, M);
    glPopMatrix();
}

void MouvementM(float M[16], int valaxe, float distance) {
    glPushMatrix();
    glLoadIdentity();
    glLoadMatrixf(M);
    
    switch (valaxe) {
        case 1: //x
            glTranslatef(distance, 0.0, 0.0);
            break;
        case 2: //y
            glTranslatef( 0.0, distance , 0.0);
            break;
            //        case 3://z
            //            glTranslatef(0.0, 0.0, distance);
            //            break;
            
    }
    glGetFloatv(GL_MODELVIEW_MATRIX, M);
    glPopMatrix();
}

/**************************************************************************************************/

/***********************************************
 ***************   Collisions   ****************
 ***********************************************
 ***********************************************/

void changerEtat(){
    etatMouvement = 1;
}

bool estChanger(){
    if (etatMouvement == 1){
        etatMouvement = 0;
        return true;
    }
    return false;
}


void setCasCollision(int c){
    casCollision = c;
}

void donnerCas(int cas){
    lastDirection = direction;
    direction = cas;
}

//
bool collisionMurs(int cas){
    Point2f boule(bouleX,bouleY);
    cout << "CAS " << cas << endl;
    cout << "CASCOLLISION" << casCollision <<endl;
    if (findIfIn(boule))
        if( cas == casCollision){
            cout << "COLLISION DETECTER AVEC LE CAS " << cas <<endl;
            return true;
        }
    
    return false;
}

//Axes : X rouge, Y Vert, - X, | Y
int checkSigne(int axis){
    //
    if (axis == 1){ //haut, bas
        if (rotationActuelleX == 0.)
            return 0;
        else if (rotationActuelleX > 0. && rotationActuelleX <=90.){
            if (collisionMurs(2)){
                                cout << "COLLISION MUR DU BAS"<< endl;
                return 0;}
            return -1;
        }else if (rotationActuelleX <0. && rotationActuelleX >= -90.){
            if (collisionMurs(3)){
                               cout << "COLLISION MUR DU HAUT"<< endl;
                return 0;}
            return 1;
            
        }
    }
    //y
    else if(axis == 2){ // droite, gauche
        if (rotationActuelleY == 0.)
            return 0;
        else if (rotationActuelleY > 0. && rotationActuelleY <=90.){
            if (collisionMurs(1)){
                                cout << "COLLISION MUR DE DROITE" << endl;
                return 0;}
            return 1;
        }else if (rotationActuelleY <0. && rotationActuelleY >= -90.){
            if (collisionMurs(0)){
                                cout << "COLLISION MUR DE GAUCHE" <<endl;
                return 0;}
            return -1;
            
        }
    }
}

void def_casCollision(int c){
    
    //    if (cas==0) // gauche
    //        if ( bouleX <= Xmin +0.4)
    //            return true;
    //    if (cas==1) // droite
    //        if (bouleX>= Xmax - 0.3)
    //            return true;
    //    if (cas==2) //bas
    //        if (bouleY>= Ymax -0.3)
    //            return true;
    //    if (cas==3) //haut
    //        if ( bouleY<= Ymin + 0.3)
    //            return true;
    //
    
    //parallele a x, cas 2(va vers le bas) et cas 3 (vers le haut)
    if(lastDirection == 2)
        setCasCollision(c);
    else if(lastDirection == 3)
        setCasCollision(c);
        
     //parallele a y, cas 0(vers gauche) et cas 1 (vers droite)
    else if (lastDirection == 0)
        setCasCollision(c);
    else if (lastDirection == 1)
        setCasCollision(c);
    
}

bool findIfIn (Point2f p){ //ou Point
    
    //equation y = coeff * x + k
    cout << endl << "findIfIn" << endl << endl;
    vector<float> tmpCoeff = vectCopy(coeffDirecteur);
    vector<float> tmpConstK = vectCopy(constK);
    
    float tmpEq = 0.;
    float delta = 0.;
    
    for (int i=0; i<A.size(); i++){
        if(tmpCoeff.back() == 9999)
            tmpEq = (tmpConstK.back());
        else
            tmpEq = ((tmpCoeff.back()) * p.x) + (tmpConstK.back());
        
        cout << "TOUR : " << i << " " << tmpCoeff.back()  << endl;
        cout << " // tmpEq : " << tmpEq << endl ;
        cout << " // P.x : " << p.x << endl;
        cout << " // P.y : " << p.y <<endl;
        
        //if collision haut/bas
        if(tmpCoeff.back() != 9999){//parallele a x
            if(direction == 2)//cas 2 (va vers le bas)
                if ((p.y) <= (tmpEq + delta) ){
                    cout << "TRUE 2" <<endl;
                    def_casCollision(direction); //0
                    return true;
                }
            else if(direction == 3) //cas 3 (vers le haut)
                if(p.y >= tmpEq - delta ){
                    cout << "TRUE 3" <<endl;
                    def_casCollision(direction); //0
                    return true;
                }
        }else if (tmpCoeff.back() == 9999){ // parallele a y
            //Collision gauche/droite
            if (direction == 0)//cas 0 (vers gauche)
                if( (p.x) <= (tmpEq + delta)){
                    cout << "TRUE 0" <<endl;
                    def_casCollision(direction);
                    return true;
                }
            else if(direction == 1)//cas 1 (vers droite)
                if (p.x >= tmpEq - delta){
                    cout << "TRUE 1" <<endl;
                    def_casCollision(direction);
                    return true;
                }
        }
        tmpCoeff.pop_back();
        tmpConstK.pop_back();
        
    }
    
    return false;
}

/**************************************************************************************************/

/***********************************************
 **************     Clavier     ****************
 ***********************************************
 ***********************************************/

//Axes : X rouge, Y Vert, - X, | Y
void clavier(unsigned char key, int x, int y){
    switch (key){
        case 'q':
        case 'Q':
            RotationM(M_plateau,2,-roty); //-10
            donnerCas(0);
            changerEtat();
            Timer(0);
            break;
        case 'd':
        case 'D':
            RotationM(M_plateau,2,roty); //10
            donnerCas(1);
            changerEtat();
            Timer(1);
            break;
        case 'z':
        case 'Z':
            RotationM(M_plateau,1,-rotx); //-10
            donnerCas(3);
            changerEtat();
            Timer(3);
            break;
        case 's':
        case 'S':
            RotationM(M_plateau,1,rotx); //10
            donnerCas(2);
            changerEtat();
            Timer(2);
            break;
        case 'p':
        case 'P':
            donnerCas(4);
            changerEtat();
            Timer(4);
            break;
            
            /*
             case 'i':
             case 'I':
             //Initialisation();
             break;
             */
        case 'a':
        case'A' :
            exit (0);
            break;
        default:
            break;
    }glutPostRedisplay();
}

/**************************************************************************************************/

/***********************************************
 ****************    Timer    ******************
 ***********************************************
 ***********************************************/


void affichageCoordBoule(int val){
    cout << val << endl;
    cout << bouleX << endl;
    cout << bouleY << endl;
    cout << "---------------------" <<endl;
}

    //Axes : X rouge, Y Vert, - X, | Y
void Timer(int value){
    
    //affichageCoordBoule(value);
    
    //on fait les deplacements en fonction de l'angle de rotation x ou y
    if (value==0){ //Va vers la gauche // q
        if(collisionMurs(0) == 0){
            MouvementM(M_boule,1,deplacementX * checkSigne(2));
            bouleX = bouleX + (deplacementX * checkSigne(2));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
        
    }else if(value==1){ // va vers la droite // d
        if(collisionMurs(1) == 0){
            MouvementM(M_boule,1,deplacementX * checkSigne(2));
            bouleX = bouleX + (deplacementX * checkSigne(2));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
        
    }else if(value==3){ //va vers le haut // z
        if(collisionMurs(3)==0){
            MouvementM(M_boule,2,deplacementY * checkSigne(1));
            bouleY = bouleY + (deplacementY * checkSigne(1));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
        
    }else if(value==2){ //va vers le bas // s
        if(collisionMurs(2)==0){
            MouvementM(M_boule,2,deplacementY * checkSigne(1));
            bouleY = bouleY + (deplacementY * checkSigne(1));
            valeurChangement = 5;
        }else
            valeurChangement = 4;
        
    }else if(value==4){
        MouvementM(M_boule,2,0);
        valeurChangement = 5;
    }
    
    glutPostRedisplay();
    
    if (valeurChangement == 5) {
        if(estChanger) //si on detecte une autre positions
            glutTimerFunc(20, Timer, direction);
        //        else {//On ne rentre jamais ici
        //            cout << "on ne rentre jamais iciii"<<endl;
        //            glutTimerFunc(20, Timer, value);}
    }else
        glutTimerFunc(20,Timer,valeurChangement);
    
}

/**************************************************************************************************/

/***********************************************
 *******Dessin, Affichage et Redimension********
 ***********************************************
 ***********************************************/

//Axes : X rouge, Y Vert, - X, | Y
void dessinScene(){
    glPushMatrix();
        glMultMatrixf(M_plateau);
        glTranslatef(-3.0,-3.0,0.0);
        def_axes();
    glPopMatrix();
    
    glPushMatrix();
        glMultMatrixf(M_plateau);
        def_plateau();
        def_boite();
        def_murfictif();
        glRotatef(-90,0,1,0);
    glPopMatrix();
    
    glPushMatrix();
    //
    glMultMatrixf(M_plateau);
        //glTranslatef(-0.3-SECTION/2., -0.3+SECTION/2., 0.3-SECTION/4.);
        glTranslatef(0., 0., 0.3-SECTION/4.);
        glMultMatrixf(M_boule);
        def_boule();
    glPopMatrix();
}

//Axes : X rouge, Y Vert, - X, | Y
void affichage(void){
    //on efface le buffer d'ecran
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_BACK, GL_LINE); // a enlever par la suite
    
    
    glPushMatrix();
        glRotatef(-80.0,1,0,0);
        //rotationActuelleX = 10.;
        dessinScene();
    glPopMatrix();
    
    //on affiche
    glutSwapBuffers();
}

void redim(int width, int height){
    glEnable(GL_DEPTH_TEST);
    glViewport(0,0,width, height);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    //          left,right,bot,top,nearVal,farVal
    //glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
    //ou
    gluPerspective(75,(float)width/height, 1.0,100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //Initialisation des matrices a Identite
    glGetFloatv(GL_MODELVIEW_MATRIX,M_plateau);
    glGetFloatv(GL_MODELVIEW_MATRIX,M_boule);
    
    //         eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ
    //gluLookAt (1.0, -2.5, -0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt    (0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void init(){

    //Taille et emplacemet de la fenetre
    glutInitWindowSize(600,600);
    glutInitWindowPosition(200,100);
    
    //Type d'affichage, Tampon de profondeur, Double buffer
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    //Creation de la premiere fenetre
    glutCreateWindow("3Dedale");
    
    remplissageCoordMur();
    
    
    //Association des callback
    glutDisplayFunc(affichage);
    glutReshapeFunc(redim);
    
    //Action Clavier
    glutKeyboardFunc(clavier);
    //glutSpecialFunc(clavierBoule);
    
    glutMainLoop();
}

void load_boule(int argc, char **argv){
    //Initialisation de GLUT
    glutInit(&argc, argv);
    init();
}

