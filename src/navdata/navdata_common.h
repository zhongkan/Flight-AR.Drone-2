/*********************************************************************
 * @file    navdata_commom.h
 * @author  kzhong1991<kzhong1991@gmail.com>
 * @date    2012-12-11
 * @brief   Common navdata configuration, the struct of navdata
 **********************************************************************/

#ifndef NAVDATA_COMMOM_H
#define NAVDATA_COMMOM_H
#include <QtCore/QtGlobal>
#include "..//common/functional.h"
#include "..//video/vision_common.h"

#define NAVDATA_HEADER                  0x55667788
#define NAVDATA_MAX_SIZE                2048
#define NAVDATA_MAX_CUSTOM_TIME_SAVE    20
#define NB_NAVDATA_DETECTION_RESULTS    4
#define NAVDATA_STATE_OFFSET            4
#define NAVDATA_SEQUENCE_NUM_OFFSET     8
#define NAVDATA_VERSION_OFFSET          12
#define NAVDATA_OPTION_TAG_OFFSET       16
#define NAVDATA_OPTION_SIZE_OFFSET      18
#define NAVDATA_OPTION_DATA_OFFSET      20

#define NAVDATA_OPTION_MASK(option) ( 1 << (option) )
#define NAVDATA_OPTION_FULL_MASK    ((1<<NAVDATA_NUM_TAGS)-1)

#if defined(_MSC_VER)
    #define _ATTRIBUTE_PACKED_
    /* Asks Visual C++ to pack structures from now on */
    #pragma pack(1)
#else
    #define _ATTRIBUTE_PACKED_  __attribute__ ((packed))
#endif


// Define constants for gyrometers handling
typedef enum {
  GYRO_X    = 0,
  GYRO_Y    = 1,
  GYRO_Z    = 2,
  NB_GYROS  = 3
} def_gyro_t;

// Define constants for accelerometers handling
typedef enum {
  ACC_X   = 0,
  ACC_Y   = 1,
  ACC_Z   = 2,
  NB_ACCS = 3
} def_acc_t;

/**
 * @struct _velocities_t
 * @brief Velocities in float format
 */
typedef struct _velocities_t {
  float x;
  float y;
  float z;
} velocities_t;

// Define navdata option tag
#define NAVDATA_OPTION_DEMO(STRUCTURE,NAME,TAG)  TAG = 0,
#define NAVDATA_OPTION(STRUCTURE,NAME,TAG)       TAG ,
#define NAVDATA_OPTION_CKS(STRUCTURE,NAME,TAG)   NAVDATA_NUM_TAGS, TAG = 0xFFFF

typedef enum _navdata_tag_t {
    #include "..//NAVDATA/navdata_keys.h"
} navdata_tag_t;


/*===============================================Navigation Data Stream==================================================
*  +------------+-------------+-----------------+--------------++------------------------------------++----++----------+
*  |   Header   | Drone state | Sequence Number | Version flag ||               Option1              ||... || Checksum |
*  +--------------------+-----------------------+--------------++------------------------------------++----++----------+
*  | 32-bit int |    32-bit   |   32-bit int    | 32-bit int   || id:16-bit | size:16-bit | data ....||... ||          |
*  +------------+-------------+-----------------+--------------++------------------------------------++----++----------+
************************************************************************************************************************/

typedef struct _navdata_option_t
{
    quint16 tag;      /* Tag for a specific option */
    quint16 size;     /* Length of the struct */
    #if defined(_MSC_VER)
        quint8  data[1];   /* Structure  complete with the specific tag */
    #else
        quint8  data[];
    #endif
}  _ATTRIBUTE_PACKED_  navdata_option_t;

/**
 * @brief Navdata structure sent over the network.
 */
typedef struct _navdata_t {
    quint32    header;			 /*!< Always set to NAVDATA_HEADER */
    quint32    ardrone_state;    /*!< Bit mask built from def_ardrone_state_mask_t */
    quint32    sequence;         /*!< Sequence number, incremented for each sent packet */
    bool_t     vision_defined;
    navdata_option_t  options[1];
} _ATTRIBUTE_PACKED_ navdata_t;


/*----------------------------------------------------------------------------*/
/**
 * @brief Minimal navigation data for all flights.
 */
typedef struct _navdata_demo_t {
    quint16     tag;					/*!< Navdata block ('option') identifier */
    quint16     size;					/*!< set this to the size of this structure */

    quint32     ctrl_state;             /*!< Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. */
    quint32     vbat_flying_percentage; /*!< battery voltage filtered (mV) */

    float       theta;                  /*!< UAV's pitch in milli-degrees */
    float       phi;                    /*!< UAV's roll  in milli-degrees */
    float       psi;                    /*!< UAV's yaw   in milli-degrees */

    qint32      altitude;               /*!< UAV's altitude in centimeters */

    float       vx;                     /*!< UAV's estimated linear velocity */
    float       vy;                     /*!< UAV's estimated linear velocity */
    float       vz;                     /*!< UAV's estimated linear velocity */
    quint32     num_frames;			    /*!< streamed frame index */ // Not used -> To integrate in video stage.

    // Camera parameters compute by detection
    matrix33_t  detection_camera_rot;   /*!<  Deprecated ! Don't use ! */
    vector31_t  detection_camera_trans; /*!<  Deprecated ! Don't use ! */
    quint32	    detection_tag_index;    /*!<  Deprecated ! Don't use ! */

    quint32	    detection_camera_type;  /*!<  Type of tag searched in detection */

    // Camera parameters compute by drone
    matrix33_t  drone_camera_rot;		/*!<  Deprecated ! Don't use ! */
    vector31_t  drone_camera_trans;	    /*!<  Deprecated ! Don't use ! */

} _ATTRIBUTE_PACKED_ navdata_demo_t;

/*----------------------------------------------------------------------------*/
/**
 * @brief Last navdata option that *must* be included at the end of all navdata packets
 * + 6 bytes
 */
typedef struct _navdata_cks_t {
    quint16    tag;
    quint16    size;

    // Checksum for all navdatas (including options)
    quint32    cks;
} _ATTRIBUTE_PACKED_ navdata_cks_t;

/*----------------------------------------------------------------------------*/
/**
 * @brief Timestamp
 * + 6 bytes
 */
typedef struct _navdata_time_t {
    qint16   tag;
    qint16   size;

    quint32  time;  /*!< 32 bit value where the 11 most significant bits represents the seconds, */
                    /*  and the 21 least significant bits are the microseconds. */
} _ATTRIBUTE_PACKED_ navdata_time_t;


/*----------------------------------------------------------------------------*/
/**
 * @brief Raw sensors measurements
 */
typedef struct _navdata_raw_measures_t {
    quint16   tag;
    quint16   size;

  // +12 bytes
    quint16   raw_accs[NB_ACCS];    // filtered accelerometers
    qint16    raw_gyros[NB_GYROS];  // filtered gyrometers
    qint16    raw_gyros_110[2];     // gyrometers  x/y 110 deg/s
    quint32   vbat_raw;             // battery voltage raw (mV)
    quint16   us_debut_echo;
    quint16   us_fin_echo;
    quint16   us_association_echo;
    quint16   us_distance_echo;
    quint16   us_courbe_temps;
    quint16   us_courbe_valeur;
    quint16   us_courbe_ref;
    quint16   flag_echo_ini;
    // TODO:   quint16  frame_number; // from ARDrone_Magneto
    quint16   nb_echo;
    quint32   sum_echo;
    qint32    alt_temp_raw;

    qint16    gradient;

} _ATTRIBUTE_PACKED_ navdata_raw_measures_t;


typedef struct _navdata_pressure_raw_t {
    quint16   tag;
    quint16   size;

    qint32    up;
    qint16    ut;
    qint32    Temperature_meas;
    qint32    Pression_meas;

}_ATTRIBUTE_PACKED_ navdata_pressure_raw_t;

typedef struct _navdata_magneto_t {
    quint16           tag;
    quint16           size;

    qint16            mx;
    qint16            my;
    qint16            mz;
    vector31_t        magneto_raw;       // magneto in the body frame, in mG
    vector31_t        magneto_rectified;
    vector31_t        magneto_offset;
    float             heading_unwrapped;
    float             heading_gyro_unwrapped;
    float             heading_fusion_unwrapped;
    char              magneto_calibration_ok;
    quint32           magneto_state;
    float             magneto_radius;
    float             error_mean;
    float             error_var;

} _ATTRIBUTE_PACKED_ navdata_magneto_t;

typedef struct _navdata_wind_speed_t {
    quint16   tag;
    quint16   size;

    float     wind_speed;
    float     wind_angle;
    float     wind_compensation_theta;
    float     wind_compensation_phi;
    float     state_x1;
    float     state_x2;
    float     state_x3;
    float     state_x4;
    float     state_x5;
    float     state_x6;
    float     magneto_debug1;
    float     magneto_debug2;
    float     magneto_debug3;

} _ATTRIBUTE_PACKED_ navdata_wind_speed_t;

typedef struct _navdata_kalman_pressure_t{
    quint16   tag;
    quint16   size;

    float     offset_pressure;
    float     est_z;
    float     est_zdot;
    float     est_bias_PWM;
    float     est_biais_pression;
    float     offset_US;
    float     prediction_US;
    float     cov_alt;
    float     cov_PWM;
    float     cov_vitesse;
    bool_t    bool_effet_sol;
    float     somme_inno;
    bool_t    flag_rejet_US;
    float     u_multisinus;
    float     gaz_altitude;
    bool_t    Flag_multisinus;
    bool_t    Flag_multisinus_debut;

} _ATTRIBUTE_PACKED_ navdata_kalman_pressure_t;



typedef struct _navdata_phys_measures_t {
    quint16   tag;
    quint16   size;

    float     accs_temp;
    quint16   gyro_temp;
    float     phys_accs[NB_ACCS];
    float     phys_gyros[NB_GYROS];
    quint32   alim3V3;              // 3.3volt alim [LSB]
    quint32   vrefEpson;            // ref volt Epson gyro [LSB]
    quint32   vrefIDG;              // ref volt IDG gyro [LSB]

} _ATTRIBUTE_PACKED_ navdata_phys_measures_t;


typedef struct _navdata_gyros_offsets_t {
    quint16   tag;
    quint16   size;

    float     offset_g[NB_GYROS];

} _ATTRIBUTE_PACKED_ navdata_gyros_offsets_t;


typedef struct _navdata_euler_angles_t {
    quint16   tag;
    quint16   size;

    float     theta_a;
    float     phi_a;

} _ATTRIBUTE_PACKED_ navdata_euler_angles_t;


typedef struct _navdata_references_t {
    quint16   tag;
    quint16   size;

    qint32    ref_theta;
    qint32    ref_phi;
    qint32    ref_theta_I;
    qint32    ref_phi_I;
    qint32    ref_pitch;
    qint32    ref_roll;
    qint32    ref_yaw;
    qint32    ref_psi;

    float     vx_ref;
    float     vy_ref;
    float     theta_mod;
    float     phi_mod;

    float     k_v_x;
    float     k_v_y;
    quint32   k_mode;

    float     ui_time;
    float     ui_theta;
    float     ui_phi;
    float     ui_psi;
    float     ui_psi_accuracy;
    qint32    ui_seq;

} _ATTRIBUTE_PACKED_ navdata_references_t;


typedef struct _navdata_trims_t {
    quint16   tag;
    quint16   size;

    float     angular_rates_trim_r;
    float     euler_angles_trim_theta;
    float     euler_angles_trim_phi;

} _ATTRIBUTE_PACKED_ navdata_trims_t;

typedef struct _navdata_rc_references_t {
    quint16   tag;
    quint16   size;

    qint32    rc_ref_pitch;
    qint32    rc_ref_roll;
    qint32    rc_ref_yaw;
    qint32    rc_ref_gaz;
    qint32    rc_ref_ag;

} _ATTRIBUTE_PACKED_ navdata_rc_references_t;


typedef struct _navdata_pwm_t {
    quint16       tag;
    quint16       size;

    quint8        motor1;
    quint8        motor2;
    quint8        motor3;
    quint8        motor4;
    quint8        sat_motor1;
    quint8        sat_motor2;
    quint8        sat_motor3;
    quint8        sat_motor4;
    float         gaz_feed_forward;
    float         gaz_altitude;
    float         altitude_integral;
    float         vz_ref;
    qint32        u_pitch;
    qint32        u_roll;
    qint32        u_yaw;
    float         yaw_u_I;
    qint32        u_pitch_planif;
    qint32        u_roll_planif;
    qint32        u_yaw_planif;
    float         u_gaz_planif;
    quint16       current_motor1;
    quint16       current_motor2;
    quint16       current_motor3;
    quint16       current_motor4;

  //WARNING: new navdata (FC 26/07/2011)
    float         altitude_prop;
    float         altitude_der;

} _ATTRIBUTE_PACKED_ navdata_pwm_t;


typedef struct _navdata_altitude_t {
    quint16     tag;
    quint16     size;

    qint32      altitude_vision;
    float       altitude_vz;
    qint32      altitude_ref;
    qint32      altitude_raw;

    float       obs_accZ;
    float       obs_alt;
    vector31_t 	obs_x;
    quint32     obs_state;
    vector21_t	est_vb;
    quint32 	est_state ;

} _ATTRIBUTE_PACKED_ navdata_altitude_t;


typedef struct _navdata_vision_raw_t {
    quint16   tag;
    quint16   size;

    float     vision_tx_raw;
    float     vision_ty_raw;
    float     vision_tz_raw;

} _ATTRIBUTE_PACKED_ navdata_vision_raw_t;


typedef struct _navdata_vision_t {
    quint16         tag;
    quint16         size;

    quint32         vision_state;
    qint32          vision_misc;
    float           vision_phi_trim;
    float           vision_phi_ref_prop;
    float           vision_theta_trim;
    float           vision_theta_ref_prop;

    qint32          new_raw_picture;
    float           theta_capture;
    float           phi_capture;
    float           psi_capture;
    qint32          altitude_capture;
    quint32         time_capture;     // time in TSECDEC format (see config.h)
    velocities_t    body_v;

    float           delta_phi;
    float           delta_theta;
    float           delta_psi;

    quint32         gold_defined;
    quint32         gold_reset;
    float           gold_x;
    float           gold_y;

} _ATTRIBUTE_PACKED_ navdata_vision_t;


typedef struct _navdata_vision_perf_t {
    quint16     tag;
    quint16     size;

    // +44 bytes
    float       time_szo;
    float       time_corners;
    float       time_compute;
    float       time_tracking;
    float       time_trans;
    float       time_update;
    float       time_custom[NAVDATA_MAX_CUSTOM_TIME_SAVE];
} _ATTRIBUTE_PACKED_ navdata_vision_perf_t;


typedef struct _navdata_trackers_send_t {
    quint16         tag;
    quint16         size;

    qint32          locked[DEFAULT_NB_TRACKERS_WIDTH * DEFAULT_NB_TRACKERS_HEIGHT];
    screen_point_t  point[DEFAULT_NB_TRACKERS_WIDTH * DEFAULT_NB_TRACKERS_HEIGHT];

} _ATTRIBUTE_PACKED_ navdata_trackers_send_t;


typedef struct _navdata_vision_detect_t {
    quint16     tag;
    quint16     size;

    quint32     nb_detected;
    quint32     type[NB_NAVDATA_DETECTION_RESULTS];
    quint32     xc[NB_NAVDATA_DETECTION_RESULTS];
    quint32     yc[NB_NAVDATA_DETECTION_RESULTS];
    quint32     width[NB_NAVDATA_DETECTION_RESULTS];
    quint32     height[NB_NAVDATA_DETECTION_RESULTS];
    quint32     dist[NB_NAVDATA_DETECTION_RESULTS];
    float       orientation_angle[NB_NAVDATA_DETECTION_RESULTS];
    matrix33_t  rotation[NB_NAVDATA_DETECTION_RESULTS];
    vector31_t  translation[NB_NAVDATA_DETECTION_RESULTS];
    quint32     camera_source[NB_NAVDATA_DETECTION_RESULTS];

} _ATTRIBUTE_PACKED_ navdata_vision_detect_t;

typedef struct _navdata_vision_of_t {
    quint16   tag;
    quint16   size;

    float     of_dx[5];
    float     of_dy[5];

} _ATTRIBUTE_PACKED_ navdata_vision_of_t;


typedef struct _navdata_watchdog_t {
    quint16   tag;
    quint16   size;

    // +4 bytes
    qint32    watchdog;

} _ATTRIBUTE_PACKED_ navdata_watchdog_t;

typedef struct _navdata_adc_data_frame_t {
    quint16  tag;
    quint16  size;

    quint32  version;
    quint8   data_frame[32];

} _ATTRIBUTE_PACKED_ navdata_adc_data_frame_t;

typedef struct _navdata_video_stream_t {
    quint16     tag;
    quint16     size;

    quint8      quant;			// quantizer reference used to encode frame [1:31]
    quint32     frame_size;		// frame size (bytes)
    quint32     frame_number;	// frame index
    quint32     atcmd_ref_seq;  // atmcd ref sequence number
    quint32     atcmd_mean_ref_gap;	// mean time between two consecutive atcmd_ref (ms)
    float       atcmd_var_ref_gap;
    quint32     atcmd_ref_quality; // estimator of atcmd link quality

    // drone2
    quint32     out_bitrate;     // measured out throughput from the video tcp socket
    quint32     desired_bitrate; // last frame size generated by the video encoder

    // misc temporary data
    qint32      data1;
    qint32      data2;
    qint32      data3;
    qint32      data4;
    qint32      data5;

    // queue usage
    quint32     tcp_queue_level;
    quint32     fifo_queue_level;

} _ATTRIBUTE_PACKED_ navdata_video_stream_t;

typedef enum
{
    NAVDATA_HDVIDEO_STORAGE_FIFO_IS_FULL = (1<<0),
    NAVDATA_HDVIDEO_USBKEY_IS_PRESENT    = (1<<8),
    NAVDATA_HDVIDEO_USBKEY_IS_RECORDING  = (1<<9),
    NAVDATA_HDVIDEO_USBKEY_IS_FULL       = (1<<10)

}_navdata_hdvideo_states_t;


typedef struct _navdata_hdvideo_stream_t {
    quint16     tag;
    quint16     size;

    quint32     hdvideo_state;
    quint32     storage_fifo_nb_packets;
    quint32     storage_fifo_size;
    quint32     usbkey_size;           /*! USB key in kbytes - 0 if no key present */
    quint32     usbkey_freespace;      /*! USB key free space in kbytes - 0 if no key present */
    quint32     frame_number;          /*! 'frame_number' PaVE field of the frame starting to be encoded for the HD stream */
    quint32     usbkey_remaining_time; /*! time in seconds */

} _ATTRIBUTE_PACKED_ navdata_hdvideo_stream_t;


typedef struct _navdata_games_t {
    quint16     tag;
    quint16     size;
    quint32     double_tap_counter;
    quint32     finish_line_counter;

} _ATTRIBUTE_PACKED_ navdata_games_t;

typedef struct _navdata_wifi_t {
    quint16     tag;
    quint16     size;
    quint32     link_quality;

} _ATTRIBUTE_PACKED_ navdata_wifi_t;

typedef struct navdata_zimmu_3000_t {
    quint16    tag;
    quint16    size;

    qint32     vzimmuLSB;
    float      vzfind;

}_ATTRIBUTE_PACKED_ navdata_zimmu_3000_t;


/**
 * @struct _navdata_unpacked_t
 * @brief Decoded navigation data.
*/
typedef struct _navdata_unpacked_t
{
    quint32         nd_seq;
    quint32         ardrone_state;
    bool_t          vision_defined;
    quint32         last_navdata_refresh;  /*! mask showing which block was refreshed when receiving navdata */

    #define NAVDATA_OPTION_DEMO(STRUCTURE,NAME,TAG)  STRUCTURE NAME ;
    #define NAVDATA_OPTION(STRUCTURE,NAME,TAG)       STRUCTURE NAME ;
    #define NAVDATA_OPTION_CKS(STRUCTURE,NAME,TAG)
    #include "..//NAVDATA/navdata_keys.h"

}  _ATTRIBUTE_PACKED_  navdata_unpacked_t;

#if defined(_MSC_VER)
    /* Go back to default packing policy */
    #pragma pack()
#endif

#endif // NAVDATA_COMMOM_H
