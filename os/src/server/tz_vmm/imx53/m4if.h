/*
 * \brief  Driver for the Multi Master Multi Memory Interface
 * \author Stefan Kalkowski
 * \date   2012-11-06
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _SRC__SERVER__VMM__INCLUDE__M4IF_H_
#define _SRC__SERVER__VMM__INCLUDE__M4IF_H_

/* Genode includes */
#include <util/mmio.h>

class M4if : Genode::Mmio
{
	private:

		struct Wm_reg0_ddr0_start : public Register<0xec, 32>
		{
			struct Addr   : Bitfield<0,20> {};
			struct Enable : Bitfield<31,1> {};
		};

		struct Wm_reg0_ddr0_end : public Register<0x10c, 32>
		{
			struct Addr   : Bitfield<0,20> {};
		};

		struct Wm_reg0_irq : public Register<0x114, 32>
		{
			struct Status_ddr0 : Bitfield<6,1>  {};
			struct Enable      : Bitfield<31,1> {};
		};

		struct Wm_reg0_addr : public Register<0x118, 32> {};

	public:

		M4if(Genode::addr_t const base) : Genode::Mmio(base) {}

		void set_region(Genode::addr_t addr, Genode::size_t size)
		{
			write<Wm_reg0_ddr0_end::Addr>((addr+size-1) >> 12);
			write<Wm_reg0_ddr0_start>(
			    Wm_reg0_ddr0_start::Addr::bits(addr >> 12) |
			    Wm_reg0_ddr0_start::Enable::bits(1));
			write<Wm_reg0_irq::Enable>(1);
		}

		void ack_irq() { write<Wm_reg0_irq::Status_ddr0>(1); }

		Genode::addr_t violation_addr() { return read<Wm_reg0_addr>(); }
};

#endif /* _SRC__SERVER__VMM__INCLUDE__M4IF_H_ */
