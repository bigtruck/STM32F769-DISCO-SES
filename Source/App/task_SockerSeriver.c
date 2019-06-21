#include <stdio.h>
#include <stdlib.h>

#include "stm32f7xx_hal.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"


static void Netif_Config(void);


#define SOCKET_SERIVER_PORT  5000

uint8_t data_buffer[256];

void task_SocketSeriver( void *parameter )
{
  GPIO_InitTypeDef  GPIO_InitStruct;
	struct sockaddr_in server_addr;
	struct sockaddr_in conn_addr;
	int sock_fd;				/* server socked */
	int sock_conn;			/* request socked */
	socklen_t addr_len;
	int err;
	int length;
	int count = 0;
	
  __GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  GPIO_InitStruct.Pin   = GPIO_PIN_5;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init( GPIOJ, &GPIO_InitStruct );
  GPIOJ->BSRR = (uint32_t)GPIO_PIN_5 << 16;

  tcpip_init(NULL, NULL);
  Netif_Config();

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		debug_printf("failed to create sock_fd!\n");
		vTaskDelete(0);
        return ;
	}
	
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family      = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port        = htons(SOCKET_SERIVER_PORT);
	
	err = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (err < 0) {
		vTaskDelete(0);
        return ;
	}
	
	err = listen(sock_fd, 1);
	if (err < 0) {
		vTaskDelete(0);
        return ;
	}

	addr_len = sizeof(struct sockaddr_in);
	
	debug_printf("before accept!\n");
    GPIOJ->BSRR = (uint32_t)GPIO_PIN_5;
	sock_conn = accept(sock_fd, (struct sockaddr *)&conn_addr, &addr_len);
	debug_printf("after accept!\n");
	
	while (1)
    {
		memset(data_buffer, 0, sizeof(data_buffer));
		
		length = recv(sock_conn, (unsigned int *)data_buffer, 20, 0);
		
		debug_printf("length received %d\n", length);
		debug_printf("received string: %s\n", data_buffer);
		debug_printf("received count: %d\n", count);

		send(sock_conn, "good\r\n", 5, 0);
	}
}



struct netif gnetif; /* network interface structure */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
 
#if USE_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
}












