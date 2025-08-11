/*
 * test_hypercomplex.c - Comprehensive test suite for hypercomplex operations
 * Production-ready testing with edge cases and performance validation
 */

#include "hypercomplex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>

// Test framework macros
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s - %s\n", __func__, message); \
            return 0; \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_EQ(expected, actual, tolerance, message) \
    do { \
        if (fabsf((expected) - (actual)) > (tolerance)) { \
            printf("FAIL: %s - %s (expected: %f, actual: %f)\n", \
                   __func__, message, expected, actual); \
            return 0; \
        } \
    } while(0)

#define RUN_TEST(test_func) \
    do { \
        printf("Running %s... ", #test_func); \
        if (test_func()) { \
            printf("PASS\n"); \
            passed_tests++; \
        } else { \
            failed_tests++; \
        } \
        total_tests++; \
    } while(0)

// Global test counters
static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

/*
 * Unit Tests
 */

int test_quaternion_identity() {
    quaternion_t q, result;
    quaternion_identity(&q);
    
    TEST_ASSERT_FLOAT_EQ(1.0f, q.w, 1e-6f, "Identity w component");
    TEST_ASSERT_FLOAT_EQ(0.0f, q.x, 1e-6f, "Identity x component");
    TEST_ASSERT_FLOAT_EQ(0.0f, q.y, 1e-6f, "Identity y component");
    TEST_ASSERT_FLOAT_EQ(0.0f, q.z, 1e-6f, "Identity z component");
    
    // Test multiplication with identity
    quaternion_t test_q;
    quaternion_init(&test_q, 2.0f, 3.0f, 4.0f, 5.0f);
    
    int ret = quaternion_multiply(&test_q, &q, &result);
    TEST_ASSERT(ret == HC_SUCCESS, "Multiplication with identity should succeed");
    
    TEST_ASSERT_FLOAT_EQ(test_q.w, result.w, 1e-6f, "Identity mult preserves w");
    TEST_ASSERT_FLOAT_EQ(test_q.x, result.x, 1e-6f, "Identity mult preserves x");
    TEST_ASSERT_FLOAT_EQ(test_q.y, result.y, 1e-6f, "Identity mult preserves y");
    TEST_ASSERT_FLOAT_EQ(test_q.z, result.z, 1e-6f, "Identity mult preserves z");
    
    return 1;
}

int test_quaternion_addition() {
    quaternion_t q1, q2, result, expected;
    
    quaternion_init(&q1, 1.0f, 2.0f, 3.0f, 4.0f);
    quaternion_init(&q2, 5.0f, 6.0f, 7.0f, 8.0f);
    quaternion_init(&expected, 6.0f, 8.0f, 10.0f, 12.0f);
    
    int ret = quaternion_add(&q1, &q2, &result);
    TEST_ASSERT(ret == HC_SUCCESS, "Addition should succeed");
    
    TEST_ASSERT_FLOAT_EQ(expected.w, result.w, 1e-6f, "Addition w component");
    TEST_ASSERT_FLOAT_EQ(expected.x, result.x, 1e-6f, "Addition x component");
    TEST_ASSERT_FLOAT_EQ(expected.y, result.y, 1e-6f, "Addition y component");
    TEST_ASSERT_FLOAT_EQ(expected.z, result.z, 1e-6f, "Addition z component");
    
    return 1;
}

int test_quaternion_multiplication() {
    // Test basic multiplication: i * j = k
    quaternion_t i, j, k, result;
    
    quaternion_init(&i, 0.0f, 1.0f, 0.0f, 0.0f);  // i
    quaternion_init(&j, 0.0f, 0.0f, 1.0f, 0.0f);  // j
    quaternion_init(&k, 0.0f, 0.0f, 0.0f, 1.0f);  // k
    
    int ret = quaternion_multiply(&i, &j, &result);
    TEST_ASSERT(ret == HC_SUCCESS, "i * j should succeed");
    
    TEST_ASSERT_FLOAT_EQ(k.w, result.w, 1e-6f, "i*j=k w component");
    TEST_ASSERT_FLOAT_EQ(k.x, result.x, 1e-6f, "i*j=k x component");
    TEST_ASSERT_FLOAT_EQ(k.y, result.y, 1e-6f, "i*j=k y component");
    TEST_ASSERT_FLOAT_EQ(k.z, result.z, 1e-6f, "i*j=k z component");
    
    // Test j * i = -k
    ret = quaternion_multiply(&j, &i, &result);
    TEST_ASSERT(ret == HC_SUCCESS, "j * i should succeed");
    
    TEST_ASSERT_FLOAT_EQ(-k.w, result.w, 1e-6f, "j*i=-k w component");
    TEST_ASSERT_FLOAT_EQ(-k.x, result.x, 1e-6f, "j*i=-k x component");
    TEST_ASSERT_FLOAT_EQ(-k.y, result.y, 1e-6f, "j*i=-k y component");
    TEST_ASSERT_FLOAT_EQ(-k.z, result.z, 1e-6f, "j*i=-k z component");
    
    return 1;
}

int test_quaternion_conjugate() {
    quaternion_t q, result, expected;
    
    quaternion_init(&q, 1.0f, 2.0f, 3.0f, 4.0f);
    quaternion_init(&expected, 1.0f, -2.0f, -3.0f, -4.0f);
    
    int ret = quaternion_conjugate(&q, &result);
    TEST_ASSERT(ret == HC_SUCCESS, "Conjugate should succeed");
    
    TEST_ASSERT_FLOAT_EQ(expected.w, result.w, 1e-6f, "Conjugate w component");
    TEST_ASSERT_FLOAT_EQ(expected.x, result.x, 1e-6f, "Conjugate x component");
    TEST_ASSERT_FLOAT_EQ(expected.y, result.y, 1e-6f, "Conjugate y component");
    TEST_ASSERT_FLOAT_EQ(expected.z, result.z, 1e-6f, "Conjugate z component");
    
    return 1;
}

int test_quaternion_norm() {
    quaternion_t q;
    
    // Test unit quaternion
    quaternion_init(&q, 1.0f, 0.0f, 0.0f, 0.0f);
    float norm = quaternion_norm(&q);
    TEST_ASSERT_FLOAT_EQ(1.0f, norm, 1e-6f, "Unit quaternion norm");
    
    // Test 3-4-5 right triangle analog
    quaternion_init(&q, 0.0f, 3.0f, 4.0f, 0.0f);
    norm = quaternion_norm(&q);
    TEST_ASSERT_FLOAT_EQ(5.0f, norm, 1e-6f, "3-4-5 quaternion norm");
    
    // Test known norm
    quaternion_init(&q, 1.0f, 2.0f, 3.0f, 4.0f);
    norm = quaternion_norm(&q);
    float expected_norm = sqrtf(1 + 4 + 9 + 16);  // sqrt(30)
    TEST_ASSERT_FLOAT_EQ(expected_norm, norm, 1e-6f, "Known quaternion norm");
    
    return 1;
}

int test_quaternion_normalize() {
    quaternion_t q, result;
    
    quaternion_init(&q, 1.0f, 2.0f, 3.0f, 4.0f);
    
    int ret = quaternion_normalize(&q, &result);
    TEST_ASSERT(ret == HC_SUCCESS, "Normalize should succeed");
    
    // Check that normalized quaternion has unit norm
    float norm = quaternion_norm(&result);
    TEST_ASSERT_FLOAT_EQ(1.0f, norm, 1e-6f, "Normalized quaternion has unit norm");
    
    // Test zero quaternion (should fail)
    quaternion_init(&q, 0.0f, 0.0f, 0.0f, 0.0f);
    ret = quaternion_normalize(&q, &result);
    TEST_ASSERT(ret == HC_ERROR_DIVIDE_ZERO, "Zero quaternion normalization should fail");
    
    return 1;
}

int test_null_pointer_handling() {
    quaternion_t q1, q2, result;
    quaternion_init(&q1, 1.0f, 2.0f, 3.0f, 4.0f);
    quaternion_init(&q2, 5.0f, 6.0f, 7.0f, 8.0f);
    
    // Test null pointer handling for all functions
    TEST_ASSERT(quaternion_multiply(NULL, &q2, &result) == HC_ERROR_NULL_PTR, "Null q1 in multiply");
    TEST_ASSERT(quaternion_multiply(&q1, NULL, &result) == HC_ERROR_NULL_PTR, "Null q2 in multiply");
    TEST_ASSERT(quaternion_multiply(&q1, &q2, NULL) == HC_ERROR_NULL_PTR, "Null result in multiply");
    
    TEST_ASSERT(quaternion_add(NULL, &q2, &result) == HC_ERROR_NULL_PTR, "Null q1 in add");
    TEST_ASSERT(quaternion_add(&q1, NULL, &result) == HC_ERROR_NULL_PTR, "Null q2 in add");
    TEST_ASSERT(quaternion_add(&q1, &q2, NULL) == HC_ERROR_NULL_PTR, "Null result in add");
    
    TEST_ASSERT(quaternion_conjugate(NULL, &result) == HC_ERROR_NULL_PTR, "Null input in conjugate");
    TEST_ASSERT(quaternion_conjugate(&q1, NULL) == HC_ERROR_NULL_PTR, "Null result in conjugate");
    
    TEST_ASSERT(quaternion_normalize(NULL, &result) == HC_ERROR_NULL_PTR, "Null input in normalize");
    TEST_ASSERT(quaternion_normalize(&q1, NULL) == HC_ERROR_NULL_PTR, "Null result in normalize");
    
    return 1;
}

int test_encryption_decryption() {
    const char* test_data = "Hello, hypercomplex world! This is test data.";
    size_t data_len = strlen(test_data);
    
    quaternion_t key;
    quaternion_generate_key(&key, 12345ULL);
    
    // Allocate buffers
    size_t cipher_len = data_len + sizeof(hypercomplex_header_t) + 32; // Extra space
    uint8_t* ciphertext = malloc(cipher_len);
    uint8_t* decrypted = malloc(data_len + 16);
    size_t decrypted_len = data_len + 16;
    
    TEST_ASSERT(ciphertext != NULL, "Cipher buffer allocation");
    TEST_ASSERT(decrypted != NULL, "Decrypt buffer allocation");
    
    // Encrypt
    int ret = hypercomplex_encrypt_data(test_data, data_len, &key, ciphertext, &cipher_len);
    TEST_ASSERT(ret == HC_SUCCESS, "Encryption should succeed");
    
    // Decrypt
    ret = hypercomplex_decrypt_data(ciphertext, cipher_len, &key, decrypted, &decrypted_len);
    TEST_ASSERT(ret == HC_SUCCESS, "Decryption should succeed");
    TEST_ASSERT(decrypted_len == data_len, "Decrypted length matches original");
    
    // Verify data
    TEST_ASSERT(memcmp(test_data, decrypted, data_len) == 0, "Decrypted data matches original");
    
    free(ciphertext);
    free(decrypted);
    
    return 1;
}

int test_edge_cases() {
    quaternion_t q, result;
    
    // Test very small numbers
    quaternion_init(&q, 1e-10f, 1e-10f, 1e-10f, 1e-10f);
    TEST_ASSERT(quaternion_is_valid(&q), "Very small numbers should be valid");
    
    // Test very large numbers
    quaternion_init(&q, 1e10f, 1e10f, 1e10f, 1e10f);
    TEST_ASSERT(quaternion_is_valid(&q), "Very large numbers should be valid");
    
    // Test infinity (should be invalid)
    quaternion_init(&q, INFINITY, 0.0f, 0.0f, 0.0f);
    TEST_ASSERT(!quaternion_is_valid(&q), "Infinity should be invalid");
    
    // Test NaN (should be invalid)
    quaternion_init(&q, NAN, 0.0f, 0.0f, 0.0f);
    TEST_ASSERT(!quaternion_is_valid(&q), "NaN should be invalid");
    
    return 1;
}

/*
 * Performance Tests
 */

int test_performance() {
    printf("\n=== Performance Tests ===\n");
    
    const size_t iterations = 1000000;
    perf_stats_t stats;
    
    int ret = hypercomplex_benchmark(iterations, &stats);
    TEST_ASSERT(ret == HC_SUCCESS, "Benchmark should succeed");
    
    printf("Operations per second: %lu\n", stats.operations_per_second);
    printf("Average latency: %.2f ns\n", stats.average_latency_ns);
    printf("Bytes processed: %zu\n", stats.bytes_processed);
    printf("Throughput: %.2f MB/s\n", 
           (stats.bytes_processed / 1024.0 / 1024.0) / (stats.average_latency_ns * iterations / 1e9));
    
    // Basic performance expectations (adjust based on platform)
    TEST_ASSERT(stats.operations_per_second > 1000000, "Should handle at least 1M ops/sec");
    TEST_ASSERT(stats.average_latency_ns < 10000, "Average latency should be under 10μs");
    
    return 1;
}

/*
 * Integration Tests
 */

int test_quaternion_properties() {
    quaternion_t q1, q2, q3, temp1, temp2, result1, result2;
    
    quaternion_init(&q1, 1.0f, 2.0f, 3.0f, 4.0f);
    quaternion_init(&q2, 5.0f, 6.0f, 7.0f, 8.0f);
    quaternion_init(&q3, 9.0f, 10.0f, 11.0f, 12.0f);
    
    // Test associativity: (q1 * q2) * q3 = q1 * (q2 * q3)
    quaternion_multiply(&q1, &q2, &temp1);
    quaternion_multiply(&temp1, &q3, &result1);
    
    quaternion_multiply(&q2, &q3, &temp2);
    quaternion_multiply(&q1, &temp2, &result2);
    
    TEST_ASSERT_FLOAT_EQ(result1.w, result2.w, 1e-5f, "Associativity w");
    TEST_ASSERT_FLOAT_EQ(result1.x, result2.x, 1e-5f, "Associativity x");
    TEST_ASSERT_FLOAT_EQ(result1.y, result2.y, 1e-5f, "Associativity y");
    TEST_ASSERT_FLOAT_EQ(result1.z, result2.z, 1e-5f, "Associativity z");
    
    // Test conjugate properties: (q1 * q2)* = q2* * q1*
    quaternion_multiply(&q1, &q2, &temp1);
    quaternion_conjugate(&temp1, &result1);
    
    quaternion_conjugate(&q1, &temp1);
    quaternion_conjugate(&q2, &temp2);
    quaternion_multiply(&temp2, &temp1, &result2);
    
    TEST_ASSERT_FLOAT_EQ(result1.w, result2.w, 1e-5f, "Conjugate property w");
    TEST_ASSERT_FLOAT_EQ(result1.x, result2.x, 1e-5f, "Conjugate property x");
    TEST_ASSERT_FLOAT_EQ(result1.y, result2.y, 1e-5f, "Conjugate property y");
    TEST_ASSERT_FLOAT_EQ(result1.z, result2.z, 1e-5f, "Conjugate property z");
    
    return 1;
}

/*
 * Main test runner
 */

void print_test_summary() {
    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", total_tests);
    printf("Passed: %d\n", passed_tests);
    printf("Failed: %d\n", failed_tests);
    printf("Success rate: %.1f%%\n", (float)passed_tests / total_tests * 100.0f);
}

int main(int argc, char* argv[]) {
    printf("Hypercomplex Math Library Test Suite\n");
    printf("====================================\n");
    
    // Check if we're running on ARM64
#ifndef __aarch64__
    printf("WARNING: This code is optimized for ARM64 architecture\n");
#endif
    
    if (argc > 1 && strcmp(argv[1], "--benchmark") == 0) {
        perf_stats_t stats;
        printf("Running performance benchmark...\n");
        if (hypercomplex_benchmark(10000000, &stats) == HC_SUCCESS) {
            printf("Performance Results:\n");
            printf("  Operations/sec: %lu\n", stats.operations_per_second);
            printf("  Average latency: %.2f ns\n", stats.average_latency_ns);
            printf("  Bytes processed: %zu\n", stats.bytes_processed);
        }
        return 0;
    }
    
    // Run all tests
    RUN_TEST(test_quaternion_identity);
    RUN_TEST(test_quaternion_addition);
    RUN_TEST(test_quaternion_multiplication);
    RUN_TEST(test_quaternion_conjugate);
    RUN_TEST(test_quaternion_norm);
    RUN_TEST(test_quaternion_normalize);
    RUN_TEST(test_null_pointer_handling);
    RUN_TEST(test_encryption_decryption);
    RUN_TEST(test_edge_cases);
    RUN_TEST(test_quaternion_properties);
    RUN_TEST(test_performance);
    
    print_test_summary();
    
    return (failed_tests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
