#ifndef RESPONSE_EXECUTOR_H
#define RESPONSE_EXECUTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/**
 * @file response_executor.h
 * @brief Enterprise Emergency Response System - Core Response Executor
 * @author Eve.aic (Yifu Yun)
 * @version 2.1.0
 * @date 2025-10-03
 * 
 * Integrated response system for handling emergency scenarios including
 * lockdown procedures, network isolation, and evacuation protocols.
 */

// Response type enumeration
typedef enum {
    RESPONSE_LOCKDOWN = 1,           // Complete facility lockdown
    RESPONSE_NETWORK_ISOLATE,        // Network segmentation and isolation
    RESPONSE_SERVICE_FAILOVER,       // Critical service failover
    RESPONSE_EVACUATION,             // Emergency evacuation procedures
    RESPONSE_BACKUP_ACTIVATE,        // Backup system activation
    RESPONSE_COMMS_PRIORITY,         // Communication priority routing
    RESPONSE_PARTIAL_CONTAIN,        // Partial containment measures
    RESPONSE_FULL_RECOVERY           // Full system recovery
} response_type_t;

// System operation mode
typedef enum {
    MODE_NORMAL = 0,                 // Normal operation
    MODE_HEIGHTENED_SECURITY,        // Increased security posture
    MODE_EMERGENCY,                  // Emergency response active
    MODE_LOCKDOWN,                   // Full lockdown active
    MODE_RECOVERY                    // Post-emergency recovery
} system_mode_t;

// Error code definitions
typedef enum {
    RESPONSE_SUCCESS = 0,            // Operation completed successfully
    RESPONSE_ERROR_INIT_FAILED = -1, // System initialization failed
    RESPONSE_ERROR_INVALID_PARAM = -2, // Invalid parameters provided
    RESPONSE_ERROR_HARDWARE_UNAVAILABLE = -3, // Required hardware not available
    RESPONSE_ERROR_NETWORK_FAILURE = -4, // Network operation failed
    RESPONSE_ERROR_ACCESS_DENIED = -5, // Insufficient permissions
    RESPONSE_ERROR_TIMEOUT = -6,     // Operation timed out
    RESPONSE_ERROR_CRITICAL_FAILURE = -99 // Critical system failure
} response_error_t;

// Authorization level definitions
typedef enum {
    AUTH_LEVEL_1 = 1,                // Basic staff
    AUTH_LEVEL_2 = 2,                // Research personnel  
    AUTH_LEVEL_3 = 3,                // Security staff
    AUTH_LEVEL_4 = 4,                // Department heads
    AUTH_LEVEL_5 = 5                 // Executive/O5 council
} auth_level_t;

// Integrated response parameters structure
typedef struct {
    response_type_t type;             // Type of response to execute
    uint8_t severity;                 // Severity level (1-10 scale)
    uint32_t target_zones;            // Bitmask of target zones
    uint32_t duration;                // Duration in seconds
    auth_level_t auth_level;          // Required authorization level
    char trigger_event[64];           // Event that triggered this response
    uint64_t timestamp;               // Unix timestamp of request
    uint32_t retry_count;             // Number of retry attempts allowed
    uint16_t timeout_seconds;         // Operation timeout in seconds
} integrated_response_t;

// Execution result structure
typedef struct {
    uint64_t response_id;             // Unique identifier for this response
    int32_t overall_result;           // Overall success/failure code
    uint64_t start_time;              // Start time (Unix timestamp)
    uint64_t end_time;                // End time (Unix timestamp)
    uint32_t sub_operations;          // Total number of sub-operations
    uint32_t success_count;           // Number of successful operations
    uint32_t failed_count;            // Number of failed operations
    uint32_t warning_count;           // Number of operations with warnings
    system_mode_t system_mode;        // System mode during execution
    char status_summary[512];         // Human-readable status summary
    char error_details[256];          // Detailed error information (if any)
} execution_report_t;

// System configuration structure
typedef struct {
    uint16_t max_response_time;       // Maximum response time in seconds
    uint8_t max_retry_attempts;       // Maximum number of retry attempts
    bool enable_emergency_override;   // Allow emergency override
    bool enable_auto_recovery;        // Enable automatic recovery
    uint16_t health_check_interval;   // Health check interval in seconds
} system_config_t;

// ============================================================================
// PUBLIC API FUNCTION DECLARATIONS
// ============================================================================

/**
 * @brief Initialize the integrated response system
 * 
 * This function initializes all subsystems including network, access control,
 * and hardware interfaces. Must be called before any other functions.
 * 
 * @return RESPONSE_SUCCESS on success, appropriate error code on failure
 */
int32_t re_init_integrated(void);

/**
 * @brief Execute an integrated response procedure
 * 
 * Processes the provided response request and coordinates all required
 * subsystems to execute the emergency procedure.
 * 
 * @param response Pointer to the response parameters structure
 * @return RESPONSE_SUCCESS on success, error code on failure
 */
int32_t re_execute_integrated(const integrated_response_t* response);

/**
 * @brief Get the last execution report
 * 
 * Returns a pointer to the most recent execution report. The report
 * is maintained internally and should not be freed by the caller.
 * 
 * @return Pointer to the last execution report, NULL if no report available
 */
execution_report_t* re_get_last_report(void);

/**
 * @brief Execute emergency sequence
 * 
 * Immediate execution of emergency procedures bypassing normal checks.
 * Used for critical situations requiring instant response.
 * 
 * @param emergency_level Emergency level (1-10, higher = more critical)
 */
void re_emergency_sequence(uint8_t emergency_level);

/**
 * @brief Check if subsystem is ready
 * 
 * Verifies that all required subsystems are operational and ready
 * to execute response procedures.
 * 
 * @return true if system is ready, false otherwise
 */
bool re_subsystem_ready(void);

/**
 * @brief Update system configuration
 * 
 * Applies new configuration parameters to the response system.
 * 
 * @param config Pointer to new configuration structure
 * @return RESPONSE_SUCCESS on success, error code on failure
 */
int32_t re_update_config(const system_config_t* config);

/**
 * @brief Get current system status
 * 
 * Retrieves the current operational status and mode of the system.
 * 
 * @return Current system mode
 */
system_mode_t re_get_system_status(void);

/**
 * @brief Cleanup system resources
 * 
 * Safely shuts down all subsystems and releases allocated resources.
 * Should be called during system shutdown.
 */
void re_cleanup_resources(void);

/**
 * @brief Validate response parameters
 * 
 * Checks if the provided response parameters are valid and within
 * acceptable ranges.
 * 
 * @param response Pointer to response parameters to validate
 * @return true if parameters are valid, false otherwise
 */
bool re_validate_parameters(const integrated_response_t* response);

#endif // RESPONSE_EXECUTOR_H