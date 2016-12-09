//
//  VectorTest.h
//  opengl
//
//  Created by Josiah Ebhomenye on 05/12/2016.
//  Copyright (c) 2016 Josiah Ebhomenye. All rights reserved.
//

#ifndef opengl_VectorTest_h
#define opengl_VectorTest_h

#include <cpptest-suite.h>
#include <cpptest-assert.h>
#include "core.h"

class VectorTestSuite : public Test::Suite{
public:
    VectorTestSuite(){
        TEST_ADD(VectorTestSuite::testInvert);
        TEST_ADD(VectorTestSuite::testLength);
        TEST_ADD(VectorTestSuite::testNormalize);
        TEST_ADD(VectorTestSuite::testVectorAddition)
        TEST_ADD(VectorTestSuite::testVectorSubtraction);
        TEST_ADD(VectorTestSuite::testScalaProduct);
        TEST_ADD(VectorTestSuite::testVectorProduct);
    }
private:
    void testInvert();
    void testLength();
    void testNormalize();
    void testVectorAddition();
    void testVectorSubtraction();
    void testScalaProduct();
    void testVectorProduct();
    
};

void VectorTestSuite::testInvert(){
    Vector v(1.5f, 0.0f, 0.72f);
    v = -v;
    Vector expected{-1.5f, 0.0f, -0.72f};
    TEST_ASSERT_MSG(v == expected, "vector v must be inverted");
    
    v.invert();
    expected = Vector(1.5f, 0.0f, 0.72f);
    TEST_ASSERT_MSG(v == expected, "vector v must be back to its original state");
    
    std::cout << "test Invert passed\n";
}

void VectorTestSuite::testLength(){
    Vector v(3.0f, 4.0f, 1.0);
    
    real m = v();
    TEST_ASSERT_DELTA_MSG(m, real_sqrt(26.0f), epsilon, "magnitude of vector v is not as expected");
    
}

void VectorTestSuite::testNormalize(){
    Vector v{3.0f, 2.0f, 6.0f};
    
    Vector expected{3.0f/7.0f, 2.0f/7.0f, 6.0f/7.0f};
    
    v.normalize();
    
    TEST_ASSERT_MSG(v == expected, "normailization of vector v failed");
}

void VectorTestSuite::testVectorAddition(){
    Vector v1{3.0f, 1.0f, -2.0f};
    Vector v2{2.0f, -1.0f, 5.0f};
    
    Vector v3 = v1 + v2;
    Vector expected{5.0f, 0.0f, 3.0f};
    
    TEST_ASSERT_MSG(v3 == expected, "vector addition failed");
    
    v1 += Vector{1.0f, 2.0f, 3.0f};
    expected = Vector{4.0f, 3.0f, 1.0f};
    
    TEST_ASSERT_MSG(v1 == expected, " vector addition assignment failed");
}

void VectorTestSuite::testVectorSubtraction(){
    Vector v1{3.0f, 1.0f, -2.0f};
    Vector v2{2.0f, -1.0f, 5.0f};
    
    Vector v3 = v1 - v2;
    
    TEST_ASSERT_MSG((v3 == Vector{1.0f, 2.0f, -7.0f}), "vector subtraction failed");
    
    v1 -= Vector{1.0f, 2.0f, 3.0f};
    
    TEST_ASSERT_MSG((v1 == Vector{2.0f, -1.0f, -5.0f}), " vector subtraction assignment failed");
}

void VectorTestSuite::testScalaProduct(){
    Vector v1{3.0f, 1.0f, -2.0f};
    Vector v2{2.0f, -1.0f, 5.0f};
    
    real expected = -5.0;
    real actual = v1.dotProduct(v2);
    TEST_ASSERT_MSG(expected == actual, "vector scala project failed");
}

void VectorTestSuite::testVectorProduct(){
    Vector a{5.0f, -6.0f, 0.0f};
    Vector b{1.0f, 2.0f, 3.0f};
    
    Vector c = a * b;
    Vector expected{-18.0f, -15.0f, 16.0f};
    TEST_ASSERT_MSG(c == expected, "vector product test failed");
    
    a *= b;
    TEST_ASSERT_MSG(a == c, "vector product assigment test failed");
    std::cout << "test vector product passed\n";
    
}


#endif
