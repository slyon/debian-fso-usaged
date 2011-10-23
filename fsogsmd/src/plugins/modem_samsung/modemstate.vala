/*
 * Copyright (C) 2011 Simon Busch <morphis@gravedo.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

namespace Samsung
{
    public static class ModemState
    {
        public static uint8 signal_strength = 0;
        public static uint32 cid;
        public static uint16 lac;
        public static uint8 power_state = 0;
        public static uint8 sim_icc_type = 0;
        public static SamsungIpc.Security.SimStatus sim_status = SamsungIpc.Security.SimStatus.INITIALIZING;
    }
}

