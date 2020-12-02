#!/usr/bin/env python3
import SitcpRbcp

def main():
    rbcp = SitcpRbcp.SitcpRbcp()
    #rbcp.set_veriy_mode()
    rbcp.set_timeout(1.0)
    for address in range(0x10, 0x2F + 1):
        data = rbcp.read_register_f('192.168.10.16', address, '>B')[0] # [0]: (return tuple)
        print('0x %02X %d' % (address, data))

if __name__ == '__main__':
    main()
