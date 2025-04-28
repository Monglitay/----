#include "key.h"

void Key_Init(Key_Initstruct *key)
{
    keys[key_counts].key_iidx = key->key_iidx;
    keys[key_counts].key_port = key->key_port;
    keys[key_counts].key_irqn = key->key_irqn;
    keys[key_counts].key_cb = key->key_cb;
    key_counts++;
    NVIC_EnableIRQ(key->key_irqn);
}

void GROUP1_IRQHandler(void)
{
    
    for(int i=0; i<key_counts ; i++)
    {
        if(DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1) == keys[i].key_iidx)
        {
            keys[i].key_cb();
            break;
        }
    }
}