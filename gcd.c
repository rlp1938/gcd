
/*      gcd.c
 *
 *	Copyright 2015 Bob Parker <rlp1938@gmail.com>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *	MA 02110-1301, USA.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <limits.h>
#include <linux/limits.h>
#include <libgen.h>
#include <errno.h>
#include <math.h>
#include "fileops.h"

#define PI 3.14159265358979323846

int main(int argc, char **argv)
{
	// max precision by using all long doubles
	long double lat1, lon1, lat2, lon2, lat1r, lon1r, lat2r, lon2r;
	const long double torad = PI / 180.;
	const long double earthrad = 6371000;	// get result in meters.
	long double a, c, d;
	if (argc != 3) {
		dowrite(2,
		"Usage: gcd latitude1,longitude1 latitude2,longitude2\n"
		"All in degrees and decimal fractions.\n"
		"Northern latitudes and Eastern longitudes to be unsigned,\n"
		"Southern latitudes and Western longitudes to be negative.\n"
	"The space after the comma separating lat, long pairs is optional\n"
		"This format suits the output of GPS displays on smart\n"
		"devices and web apps like Google Maps.\n");
		exit(EXIT_FAILURE);
	}
	errno = 0;

	char *latlong1 = strdup(argv[1]);
	char *latlong2 = strdup(argv[2]);
	char *sep = strchr(latlong1, ',');
	if (!sep) {
		fprintf(stderr, "Malformed argv[1]: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	*sep = '\0';
	lat1 = strtold(latlong1, NULL);
	if (errno) {
		perror(latlong1);
		exit(EXIT_FAILURE);
	}
	char *lonstr = sep + 1;
	lon1 = strtold(lonstr, NULL);
	if (errno) {
		perror(lonstr);
		exit(EXIT_FAILURE);
	}

	sep = strchr(latlong2, ',');
	if (!sep) {
		fprintf(stderr, "Malformed argv[2]: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	*sep = '\0';

	lat2 = strtold(latlong2, NULL);
	if (errno) {
		perror(latlong2);
		exit(EXIT_FAILURE);
	}
	lonstr = sep + 1;
	lon2 = strtold(lonstr, NULL);
	if (errno) {
		perror(lonstr);
		exit(EXIT_FAILURE);
	}
	lat1r = lat1 * torad;
	lon1r = lon1 * torad;
	lat2r = lat2 * torad;
	lon2r = lon2 * torad;

	// Haversine
	long double deltalat = lat2r - lat1r;
	long double deltalon = lon2r - lon1r;
	long double thesinlat = sinl(deltalat/2.);
	long double thesinlon = sinl(deltalon/2.);
	a = thesinlat * thesinlat + cosl(lat1r) * cosl(lat2r) * thesinlon *
					thesinlon;
	c = 2. * atan2l(sqrtl(a), sqrtl(1 - a));
	d = floorl(earthrad * c + 0.5);	// round to nearest meter.
	fprintf(stdout, "Distance is: %d meters.\n", (int) d);
	free(latlong2);
	free(latlong1);
	return 0;
}
