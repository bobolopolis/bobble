/*
 * bobble - Dashcam based on OpenCV
 * Copyright 2014 Robert Joslyn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPSD_CLIENT_HPP
#define GPSD_CLIENT_HPP

#include <mutex>
#include <thread>
#include "libgpsmm.h"

struct g_gps_data_t {
    double latitude = 0; // Latitude in degrees.
    double longitude = 0; // Longitude in degrees.
    double speed = 0; // Ground speed in meters per second.
};

extern g_gps_data_t g_gps_data;
extern std::mutex g_gps_data_mutex;

void gps_thread();

#endif
