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

#include "gpsd_client.hpp"

g_gps_data_t g_gps_data;
std::mutex g_gps_data_mutex;

void gps_thread() {
    gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);
    if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
        printf("No gpsd running.\n");
        return;
    }
    while (true) {
        struct gps_data_t *newdata;

        if (!gps_rec.waiting(50000000)) {
            continue;
        }

        if ((newdata = gps_rec.read()) == NULL) {
            printf("Read error.\n");
            return;
        } else {
            if (newdata->set & SPEED_SET) {
                g_gps_data_mutex.lock();
                g_gps_data.speed = newdata->fix.speed;
                g_gps_data_mutex.unlock();
            }
            if (newdata->set & LATLON_SET) {
                g_gps_data_mutex.lock();
                g_gps_data.latitude = newdata->fix.latitude;
                g_gps_data.longitude = newdata->fix.longitude;
                g_gps_data_mutex.unlock();

            }
        }
    }
}
