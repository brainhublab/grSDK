#include "renderer.h"


// helpers
bool showPoint( float x, float y, float z )
{
        glPointSize( 5.f );
        glBegin( GL_POINTS );
        glColor3ub( 100, 200, 0 );
        glVertex3f( x, y, z );
        glEnd( );

        return true;
}

// class methods
Renderer::Renderer()
{
        initGL( );

        setViewport( SCREEN_WIDTH, SCREEN_HEIGHT );

}

bool Renderer::initGL()
{
        glShadeModel( GL_SMOOTH );
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
        glClearDepth( 1.0f );
        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

        return true;
}

bool Renderer::setViewport( int width, int height )
{
        GLfloat ratio;

        if ( height == 0 )
        {
                height = 1;
        }

        ratio = GLfloat( width ) / GLfloat( height );
        glViewport( 0, 0, GLsizei( width), GLsizei( height ));
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity( );

        gluPerspective( 35.f , ratio, 0.1f, 100.0f );

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity( );


//        glViewport(0,0, width,height);
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        gluPerspective( 45.0, (float)width/height, 0.01, 100.0 );


//        glMatrixMode( GL_MODELVIEW );
//        glLoadIdentity( );

        data.screen.width = width;
        data.screen.height = height;

        return true;
}

int Renderer::getWidth()
{
        return data.screen.width;
}

int Renderer::getHeight()
{
        return data.screen.height;
}
;

bool Renderer::update()
{
    return true;
}

bool Renderer::renderTrajectory( Arm *arm, float angleX, float angleY, float angleZ, bool isLeft )
{
        const std::vector< std::map< char, float> > & trajectory = arm->getTrajectoryAngles();
        for ( std::map< char, float > angles : trajectory )
        {
                glLoadIdentity( );

                // pos on screen
                if ( isLeft )
                {
                        glTranslatef( -data.arm.initial_pos.x, data.arm.initial_pos.y, data.arm.initial_pos.z );
                }
                else
                {
                        glTranslatef( data.arm.initial_pos.x, data.arm.initial_pos.y, data.arm.initial_pos.z );
                }

                glRotatef( angleX, 1.f, 0.f, 0.f );
                glRotatef( angleY, 0.f, 1.f, 0.f );
                glRotatef( angleZ, 0.f, 0.f, 1.f );


                glRotatef( angles['X'], 1.f, 0.f, 0.f );
                glRotatef( angles['Y'], 0.f, 1.f, 0.f );
                glRotatef( angles['Z'], 0.f, 0.f, 1.f );

                glTranslatef( 0.f, data.arm.length, 0.f );
                createColorSphere( 0.03f, 10, 10, 0, 200, 0 );
                glTranslatef( 0.f, -data.arm.length, 0.f );
        }

        return true;
}

bool Renderer::drawScene(float angleX, float angleY, float angleZ)
{
        glLoadIdentity( );

        glColor3ub(0, 200, 0);
        float screenFloatX = 10.3f ,
                        screenFloatY = 5.5f; // todo: where can i receive these things?
        float halfScreenX = screenFloatX/2.f, halfScreenY = screenFloatY/2.f;
        float insidePointX = halfScreenX / 1.3f;
        float insidePointY = halfScreenY / 1.3f;

        glTranslatef( 0, 0, data.initial_depth );

        // rotating to current user view
//  glRotatef( angleX, 1.f, 0.f, 0.f );
//  glRotatef( angleY, 0.f, 1.f, 0.f );
//  glRotatef( angleZ, 0.f, 0.f, 1.f );

        // left piece of scene
        glBegin(GL_LINE_LOOP);
        glVertex3f(-halfScreenX, -halfScreenY, 0);
        glVertex3f(-insidePointX, -insidePointY, data.scene_depth);
        glVertex3f(-insidePointX, insidePointY, data.scene_depth);
        glVertex3f(-halfScreenX, halfScreenY, 0);
        glEnd();
        // right piece of scene
        glBegin(GL_LINE_LOOP);
        glVertex3f(halfScreenX, -halfScreenY, 0);
        glVertex3f(insidePointX, -insidePointY, data.scene_depth);
        glVertex3f(insidePointX, insidePointY, data.scene_depth);
        glVertex3f(halfScreenX, halfScreenY, 0);
        glEnd();


        // top piece of scene
        glBegin(GL_LINE_LOOP);
        glVertex3f(-halfScreenX, halfScreenY, 0);
        glVertex3f(halfScreenX, halfScreenY, 0);
        glVertex3f(insidePointX, insidePointY, data.scene_depth);
        glVertex3f(-insidePointX, insidePointY, data.scene_depth);
        glEnd();


        // bottom piece of scene
        glBegin(GL_LINE_LOOP);
        glVertex3f(-halfScreenX, -halfScreenY, 0);
        glVertex3f(halfScreenX, -halfScreenY, 0);
        glVertex3f(insidePointX, -insidePointY, data.scene_depth);
        glVertex3f(-insidePointX, -insidePointY, data.scene_depth);
        glEnd();

        // center of scene
        glBegin(GL_LINE_LOOP);
        glVertex3f(-insidePointX, insidePointY, data.scene_depth);
        glVertex3f(insidePointX, insidePointY, data.scene_depth);
        glVertex3f(insidePointX, -insidePointY, data.scene_depth);
        glVertex3f(-insidePointX, -insidePointY, data.scene_depth);
        glEnd();


        return true;
}

bool Renderer::renderArm( Node *arm, float angleX, float angleY, float angleZ, bool isLeft )
{
        glLoadIdentity( );

        // pos on screen
        if ( isLeft )
        {
                glTranslatef( -data.arm.initial_pos.x, data.arm.initial_pos.y, data.arm.initial_pos.z );
        }
        else
        {
                glTranslatef( data.arm.initial_pos.x, data.arm.initial_pos.y, data.arm.initial_pos.z );
        }

        glRotatef( angleX, 1.f, 0.f, 0.f );
        glRotatef( angleY, 0.f, 1.f, 0.f );
        glRotatef( angleZ, 0.f, 0.f, 1.f );

        /** draw arm **/
        glRotatef(( *arm ).angleX, 1.f, 0.f, 0.f );
        glRotatef(( *arm ).angleY, 0.f, 1.f, 0.f );
        glRotatef(( *arm ).angleZ, 0.f, 0.f, 1.f );

        createFrustum( data.arm.radius, data.arm.length, 0.8 );

        /** translating to hand drawing **/
        glTranslatef( 0.f, data.arm.length, 0.f );


        // rotating by Y only only for hand
        glRotatef(( *arm ).angleY, 0.f, 1.f, 0.f );

        /** draw hand **/
        struct Node *hand = &( *arm ).children[ 0 ];

        createSphere( 0.3f, 10, 10 );

        glRotatef(( *hand ).angleX, 1.f, 0.f, 0.f );
        glRotatef(( *hand ).angleY, 0.f, 1.f, 0.f );
        glRotatef(( *hand ).angleZ, 0.f, 0.f, 1.f );

        if ( isLeft )
        {
                createLeftHand( data.arm.hand_width, data.arm.hand_height, data.arm.hand_depth );
        }
        else
        {
                createRightHand( data.arm.hand_width, data.arm.hand_height, data.arm.hand_depth );
        }

        glTranslatef( 0.f, data.arm.hand_height, 0.f );

        /** draw fingers  **/

        glTranslatef( -( data.arm.hand_width * 0.4f ) + data.arm.phalange_radiuses[2], 0.f, 0.f );

        float fingerDistance = ( data.arm.hand_width * 0.8f - 4 * 2 * data.arm.phalange_radiuses[2] ) / 3 + 2 * data.arm.phalange_radiuses[2];

        if ( isLeft )
        {
                for ( int i = 3; i >= 0; i-- )
                {
                        drawFinger( hand, fingerDistance, i );
                }
        }
        else
        {
                for ( int i = 0; i < 4; i++ )
                {
                        drawFinger( hand, fingerDistance, i );
                }
        }

        //
        // thumb
        //
        if ( isLeft )
        {
                glTranslatef( -data.arm.hand_width + data.arm.phalange_radiuses[2] - 0.07f, -( data.arm.hand_height / 2 ) + data.arm.phalange_radiuses[2]/2, 0.f );
                glRotatef( 65.f, 0.f, 0.f, 1.f );
                glRotatef( 35.f, 0.f, 1.f, 0.f );
        }
        else
        {
                glTranslatef( -fingerDistance + data.arm.phalange_radiuses[2] + 0.07f, -( data.arm.hand_height / 2 ) + data.arm.phalange_radiuses[2]/2, 0.f );
                glRotatef( -65.f, 0.f, 0.f, 1.f );
                glRotatef( -35.f, 0.f, 1.f, 0.f );
        }

        drawFinger(hand, fingerDistance, 4);


        return true;
}
bool Renderer::drawFinger( Node *hand, float fingerDistance, int fingerIndex )
{
        struct Node *phalange = &( *hand ).children[ fingerIndex ];

        glPushMatrix( );

        float heightRatio = 0.f, radiusRatio = 0.f;
        while ( phalange != NULL )
        {
                glRotatef(( *phalange ).angleX, 1.f, 0.f, 0.f );
                glRotatef(( *phalange ).angleY, 0.f, 1.f, 0.f );
                glRotatef(( *phalange ).angleZ, 0.f, 0.f, 1.f );

                createFrustum( data.arm.phalange_radiuses[fingerIndex] - radiusRatio, data.arm.phalange_heights[fingerIndex] - heightRatio, 0.8 );

                createSphere( data.arm.phalange_radiuses[fingerIndex] - radiusRatio + 0.02f, 10, 10 );

                glTranslatef( 0.f, data.arm.phalange_heights[fingerIndex] - heightRatio, 0.f );

                phalange = &( *phalange ).children[ 0 ];
                heightRatio += data.arm.phalange_heights[fingerIndex]/7;
                radiusRatio += data.arm.phalange_radiuses[fingerIndex]/7;
        }

        glPopMatrix( );

        glTranslatef( fingerDistance, 0.f, 0.f );

        return true;
}

bool Renderer::createCube( float x, float y, float z )
{
        float halfX = x / 2, halfZ = z / 2;

        glBegin( GL_QUADS );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfX, y, halfZ );
        glVertex3f( halfX, 0.f, halfZ );
        glVertex3f( -halfX, 0.f, halfZ );
        glVertex3f( -halfX, y, halfZ );

        glVertex3f( halfX, y, halfZ );
        glVertex3f( halfX, y, -halfZ );
        glVertex3f( halfX, 0.f, -halfZ );
        glVertex3f( halfX, 0.f, halfZ );

        glVertex3f( -halfX, y, halfZ );
        glVertex3f( -halfX, y, -halfZ );
        glVertex3f( -halfX, 0.f, -halfZ );
        glVertex3f( -halfX, 0.f, halfZ );

        glVertex3f( halfX, y, halfZ );
        glVertex3f( halfX, y, -halfZ );
        glVertex3f( -halfX, y, -halfZ );
        glVertex3f( -halfX, y, halfZ );

        glVertex3f( halfX, 0.f, halfZ );
        glVertex3f( halfX, 0.f, -halfZ );
        glVertex3f( -halfX, 0.f, -halfZ );
        glVertex3f( -halfX, 0.f, halfZ );

        glVertex3f( halfX, y, -halfZ );
        glVertex3f( halfX, 0.f, -halfZ );
        glVertex3f( -halfX, 0.f, -halfZ );
        glVertex3f( -halfX, y, -halfZ );
        glEnd( );

        glBegin( GL_LINES );
        glColor3ub( 255, 0, 0 );

        glVertex3f( halfX, y, halfZ );
        glVertex3f( -halfX, y, halfZ );

        glVertex3f( -halfX, y, halfZ );
        glVertex3f( -halfX, y, -halfZ );

        glVertex3f( -halfX, y, -halfZ );
        glVertex3f( halfX, y, -halfZ );

        glVertex3f( halfX, y, -halfZ );
        glVertex3f( halfX, y, halfZ );

        glVertex3f( halfX, 0.f, halfZ );
        glVertex3f( -halfX, 0.f, halfZ );

        glVertex3f( -halfX, 0.f, halfZ );
        glVertex3f( -halfX, 0.f, -halfZ );

        glVertex3f( -halfX, 0.f, -halfZ );
        glVertex3f( halfX, 0.f, -halfZ );

        glVertex3f( halfX, 0.f, -halfZ );
        glVertex3f( halfX, 0.f, halfZ );

        glVertex3f( halfX, y, halfZ );
        glVertex3f( halfX, 0.f, halfZ );

        glVertex3f( -halfX, y, halfZ );
        glVertex3f( -halfX, 0.f, halfZ );

        glVertex3f( -halfX, y, -halfZ );
        glVertex3f( -halfX, 0.f, -halfZ );

        glVertex3f( halfX, y, -halfZ );
        glVertex3f( halfX, 0.f, -halfZ );
        glEnd( );

        return true;
}


bool Renderer::createRightHand( float x, float y, float z )
{
        float halfBase = 0.25f * x; // base is slightly narrower than full hand's width
        float halfTop = 0.4f * x;
        float halfDepth = 0.5 * z;

        float leftPointX = -0.45f * x, leftPointY = 0.85f * y;
        float rightPointX = 0.5f * x, rightPointY = 0.5f * y;

        // bottom hand
        glBegin( GL_TRIANGLE_FAN );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfBase, 0.f, halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );
        glVertex3f( leftPointX, leftPointY, halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( rightPointX, rightPointY, halfDepth );
        glVertex3f( halfBase, 0.f, halfDepth );
        glEnd( );

        glBegin( GL_LINE_LOOP );
        glColor3ub( 255, 0, 0 );
        glVertex3f( halfBase, 0.f, halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );
        glVertex3f( leftPointX, leftPointY, halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( rightPointX, rightPointY, halfDepth );
        glEnd( );


        // hand top
        glBegin( GL_TRIANGLE_FAN );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( leftPointX, leftPointY, -halfDepth );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glVertex3f( rightPointX, rightPointY, -halfDepth );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glEnd( );

        glBegin( GL_LINE_LOOP );
        glColor3ub( 255, 0, 0 );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( leftPointX, leftPointY, -halfDepth );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glVertex3f( rightPointX, rightPointY, -halfDepth );
        glEnd( );


        glBegin( GL_TRIANGLE_FAN );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfBase, 0.f, halfDepth );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );
        glEnd( );

        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );
        glVertex3f( leftPointX, leftPointY, halfDepth );
        glVertex3f( leftPointX, leftPointY, -halfDepth );
        glEnd( );

        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( leftPointX, leftPointY, halfDepth );
        glVertex3f( leftPointX, leftPointY, -halfDepth );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glEnd( );

        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glEnd( );

        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glVertex3f( rightPointX, rightPointY, -halfDepth );
        glVertex3f( rightPointX, rightPointY, halfDepth );
        glEnd( );

        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( rightPointX, rightPointY, -halfDepth );
        glVertex3f( rightPointX, rightPointY, halfDepth );
        glVertex3f( halfBase, 0.f, halfDepth );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glEnd( );

        return true;
}

bool Renderer::createLeftHand( float x, float y, float z )
{
        float halfBase = 0.25f * x; // base is slightly narrower than full hand's width
        float halfTop = 0.4f * x;
        float halfDepth = 0.5 * z;

        float leftPointX = -0.45f * x, leftPointY = 0.85f * y;
        float rightPointX = 0.5f * x, rightPointY = 0.5f * y;

        // bottom hand
        glBegin( GL_TRIANGLE_FAN );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfBase, 0.f, halfDepth );//0
        glVertex3f( -halfBase, 0.f, halfDepth ); //1
        glVertex3f( -rightPointX, rightPointY, halfDepth ); // 5
        glVertex3f( -halfTop, y, halfDepth ); //4
        glVertex3f( halfTop, y, halfDepth ); //3
        glVertex3f( -leftPointX, leftPointY, halfDepth ); // 2

        //glVertex3f(halfBase, 0.f, halfDepth); // todo: for what?
        glEnd( );

        glBegin( GL_LINE_LOOP );
        glColor3ub( 255, 0, 0 );
        glVertex3f( halfBase, 0.f, halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );

        glVertex3f( -rightPointX, rightPointY, halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( -leftPointX, leftPointY, halfDepth );
        glEnd( );


        // hand top
        glBegin( GL_TRIANGLE_FAN );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( -rightPointX, rightPointY, -halfDepth );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glVertex3f( -leftPointX, leftPointY, -halfDepth );
        glEnd( );

        // top
        glBegin( GL_LINE_LOOP );
        glColor3ub( 255, 0, 0 );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( -rightPointX, rightPointY, -halfDepth );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glVertex3f( -leftPointX, leftPointY, -halfDepth );
        glEnd( );


        // 0-1
        glBegin( GL_TRIANGLE_FAN );
        glColor3ub( 255, 255, 255 );
        glVertex3f( halfBase, 0.f, halfDepth );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );
        glEnd( );

        //1-2
        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( -halfBase, 0.f, -halfDepth );
        glVertex3f( -halfBase, 0.f, halfDepth );
        glVertex3f( -rightPointX, rightPointY, halfDepth );
        glVertex3f( -rightPointX, rightPointY, -halfDepth );
        glEnd( );

        // 2-3
        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( -rightPointX, rightPointY, halfDepth );
        glVertex3f( -rightPointX, rightPointY, -halfDepth );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glEnd( );

        //3-4
        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( -halfTop, y, -halfDepth );
        glVertex3f( -halfTop, y, halfDepth );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glEnd( );

        //4-5
        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( halfTop, y, halfDepth );
        glVertex3f( halfTop, y, -halfDepth );
        glVertex3f( -leftPointX, leftPointY, -halfDepth );
        glVertex3f( -leftPointX, leftPointY, halfDepth );
        glEnd( );

        // 5-6
        glBegin( GL_TRIANGLE_FAN );
        glVertex3f( -leftPointX, leftPointY, halfDepth );
        glVertex3f( -leftPointX, leftPointY, -halfDepth );
        glVertex3f( halfBase, 0.f, -halfDepth );
        glVertex3f( halfBase, 0.f, halfDepth );
        glEnd( );

        return true;
}

bool Renderer::createSphere( float radius, int slices, int stacks )
{
        GLUquadric *quad = gluNewQuadric( );
        glColor3ub( 255, 0, 0 );
        gluSphere( quad, radius, slices, stacks );
        gluDeleteQuadric( quad );

        return true;
}

bool Renderer::createColorSphere(float radius, int slices, int stacks, GLubyte r, GLubyte g, GLubyte b)
{
        GLUquadric *quad = gluNewQuadric( );
        glColor3ub( r, g, b );
        gluSphere( quad, radius, slices, stacks );
        gluDeleteQuadric( quad );

        return true;
}

bool Renderer::createCylinder( float radius, float height )
{
        if(createFrustum( radius, height, 1 ))
        {
            return true;
        }
        else
        {
            return true;
        }
}

bool Renderer::createFrustum( float radius, float height, float ratio = 1 ) // ratio : x,z(top) = ratio*x,z(bottom)
{
        float angle = 360.f / data.cylinder_sides;
        float x1, x2, z1, z2, y1, y2;

        y1 = 0.f;
        y2 = y1 + height;

        x2 = 0;
        z2 = radius;

        for ( int i = 0; i < data.cylinder_sides; i++ )
        {
                x1 = x2;
                z1 = z2;

                // rotating points
                x2 = sinf( static_cast<float>(( angle * ( i + 1 )) * ( M_PI / 180 ))) * radius;
                z2 = cosf( static_cast<float>(( angle * ( i + 1 )) * ( M_PI / 180 ))) * radius;

                // drawing bottom triangle
                glBegin( GL_TRIANGLES );
                glColor3ub( 255, 255, 255 );
                glVertex3f( 0.f, y1, 0.f );
                glVertex3f( x1, y1, z1 );
                glVertex3f( x2, y1, z2 );
                glEnd( );

                glBegin( GL_LINE_LOOP );
                glColor3ub( 255, 0, 0 );
                glVertex3f( 0.f, y1, 0.f );
                glVertex3f( x1, y1, z1 );
                glVertex3f( x2, y1, z2 );
                glEnd( );

                // drawing top triangle
                glBegin( GL_TRIANGLES );
                glColor3ub( 255, 255, 255 );
                glVertex3f( 0.f, y2, 0.f );
                glVertex3f( x1 * ratio, y2, z1 * ratio );
                glVertex3f( x2 * ratio, y2, z2 * ratio );
                glEnd( );

                glBegin( GL_LINE_LOOP );
                glColor3ub( 255, 0, 0 );
                glVertex3f( 0.f, y2, 0.f );
                glVertex3f( x1 * ratio, y2, z1 * ratio );
                glVertex3f( x2 * ratio, y2, z2 * ratio );
                glEnd( );


                // drawing side between triangles
                glBegin( GL_QUADS );
                glColor3ub( 255, 255, 255 );
                glVertex3f( x1, y1, z1 );
                glVertex3f( x2, y1, z2 );
                glVertex3f( x2 * ratio, y2, z2 * ratio );
                glVertex3f( x1 * ratio, y2, z1 * ratio );
                glEnd( );

                glBegin( GL_LINE_LOOP );
                glColor3ub( 255, 0, 0 );
                glVertex3f( x1, y1, z1 );
                glVertex3f( x2, y1, z2 );
                glVertex3f( x2 * ratio, y2, z2 * ratio );
                glVertex3f( x1 * ratio, y2, z1 * ratio );
                glEnd( );
        }

        return true;
}
