#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs152coin.h"

// This function is already implemented as an example of how to use the parser
dep_wd_t* parse_dep_wd()
{
    char* s = NULL;
    double d = 0;
    
    if (!read_string(&s)) return NULL;
    dep_wd_type type;
    if (!strcmp(s, "DEPOSIT")) {
        type = DEPOSIT;
    } else if (!strcmp(s, "WITHDRAWAL")) {
        type = WITHDRAWAL;
    } else {
        return NULL;
    }
    next_token();
    
    dep_wd_t* dw = calloc(1, sizeof(dep_wd_t));
    if (dw == NULL) {
        return NULL;
    }
    dw->type = type;
    
    if (!read_string(&s)) {
        dep_wd_free(dw);
        return NULL;
    }
    dw->addr = strdup(s);
    next_token();
    
    if (!read_double(&d)) {
        dep_wd_free(dw);
        return NULL;
    }
    dw->amount = d;
    next_token();
    
    return dw;
}

trans_t* parse_trans()
{
	char *s = NULL;
	if (!read_string(&s)) return NULL;
	if (!strcmp(s,"BEGIN_TRANSACTION")) {
		trans_t *trans = calloc(1, sizeof(trans_t));
		trans->next = NULL;
		trans->num_inputs = 0;
		allocation_check(trans,"parse_trans");
		next_token();
		if (!read_string(&s)) {
			trans_free(trans);
			fprintf(stderr,"text file corrupted");
			exit(1);
		}
		trans->id = strdup(s);
		next_token();
		if (!read_string(&s) && strcmp(s,"BEGIN_INPUTS")) {
			trans_free(trans);
			return NULL;
		} else {
			next_token();
		}
		while (strcmp(s, "END_INPUTS")) {
			if (!read_string(&s)) {
				trans_free(trans);
				return NULL;
			}

			dep_wd_t *dw = parse_dep_wd();
			trans->inputs[trans->num_inputs++] = dw;
		}	
		next_token();
		trans->output = parse_dep_wd();
		return trans;
	} else {
		return NULL;
	}
}

block_t* parse_block()
{
	char *s = NULL;

	if ((!read_string(&s)) || strcmp(s,"BEGIN_BLOCK"))
		return NULL;
	block_t *block = calloc(1,sizeof(block_t));
	allocation_check(block,"parse_block");
	next_token();
	if (!read_string(&s)) {
		block_free(block);
		return NULL;
	}
	block->miner = parse_dep_wd();
	if (!read_string(&s)) {
		block_free(block);
		return NULL;
	}
	while (!strcmp(s,"BEGIN_TRANSACTION")) {
		trans_t *trans = parse_trans();
		if (trans == NULL) {
			block_free(block);
			return NULL;
		}
		trans->next = block->trans;
		block->trans = trans;
	}
	next_token();
	return block;
		
}

blockchain_t* parse_blockchain()
{
	char *s = NULL;
	if (!read_string(&s) || strcmp(s,"BEGIN_BLOCK")) return NULL;
	blockchain_t *bchain = calloc(1,sizeof(blockchain_t));
	allocation_check(bchain,"parse_blockchain");
	while (!strcmp(s,"BEGIN_BLOCK")) {
		block_t *b = parse_block();
		if (b == NULL) {
			blockchain_free(bchain);
			return NULL;
		}
		b->next = bchain->blocks;
		bchain->blocks = b;
	}
	return bchain;
}

void dep_wd_free(dep_wd_t* dw)
{
	if (dw != NULL) {
		free(dw->addr);
		free(dw);
	}
}

void trans_free(trans_t* trans)
{
	if (trans != NULL) {
		int i = 0;
		while (trans->inputs[i] != NULL) 
			dep_wd_free(trans->inputs[i++]);
		dep_wd_free(trans->output);
		free(trans);
	}
	
}

void block_free(block_t* block)
{
	if (block == NULL) 
		return;
	else {
		if (block->miner != NULL)
			dep_wd_free(block->miner);
		if (block->trans != NULL) {
			while (block->trans != NULL) {
				trans_t *tmp = block->trans;
				block->trans = block->trans->next;
				trans_free(tmp);
			}
		}
		free(block);
	}
	
}

void blockchain_free(blockchain_t* bc)
{
	if (bc != NULL) {
		while (bc->blocks != NULL) {
			block_t *tmp = bc->blocks;
			bc->blocks = bc->blocks->next;
			block_free(tmp);
		}
		addr_bal_free(bc->addr_bal);
		dbl_spends_free(bc->dbl_spends);
		free(bc);
	}
}

void compute_stats(blockchain_t* bc)
{
	unsigned long int num_blocks = 0;
	unsigned long int num_trans = 0;
	double tot_trans_val = 0;
	addr_bal_t *bals = addr_bal_new();
	dbl_spends_t *ds = dbl_spends_new();

	if (bc == NULL) {
		fprintf(stderr,"blockchain empty\n");
		exit(1);
	}
	block_t *blocks = bc->blocks;
	while (blocks != NULL) {
		num_blocks++;
		dep_wd_t *miner = blocks->miner;
		double mbal = 0;
		if (get_balance(miner->addr,&mbal,bals))
			set_balance(miner->addr,miner->amount+mbal,bals);
		else 
			addr_bal_insert(miner->addr,miner->amount,bals);
		trans_t *trans = blocks->trans;
		while (trans != NULL) {
			num_trans++;
			dep_wd_t *output = trans->output;
			double obal = 0;
			if (get_balance(output->addr,&obal,bals))
				set_balance(output->addr,output->amount+obal,bals);
			else 
				addr_bal_insert(output->addr,output->amount,bals);
			tot_trans_val += output->amount;
			for (int i = 0; i<5; i++) {
				dep_wd_t *input = trans->inputs[i];
				if (input != NULL) {
					double ibal = 0;
					int check = get_balance(input->addr,&ibal,bals);
					if (!check || ibal > input->amount)
						add_dbl_spend(trans->id,input->addr,ds);
					else 	
						set_balance(input->addr,ibal-input->amount,bals);
				}
			}
			trans = bc->blocks->trans->next;
		}
		blocks = bc->blocks->next;
	}
	bc->num_blocks = num_blocks;
	bc->num_trans = num_trans;
	bc->avg_trans_value = tot_trans_val/num_trans;
	bc->addr_bal = bals;
	bc->dbl_spends = ds;
}	

static void print_stats(blockchain_t* bc)
{
    if (bc == NULL) return;
   

    printf("Number of blocks: %lu\n", bc->num_blocks);
    printf("Number of transactions: %lu\n", bc->num_trans);
    printf("Average transaction value: %lf\n", bc->avg_trans_value);
    printf("\n");
    
    printf("Double spends:\n");
    dbl_spends_show(bc->dbl_spends, stdout);
    printf("\n");
}

int main(int argc, char* argv[])
{
    init_parser(); 
    blockchain_t* bc = parse_blockchain();

    
    compute_stats(bc);
    print_stats(bc);
    
    for (int i = 1; i < argc; i++) {
        double bal = 0;
        if (get_balance(argv[i], &bal, bc->addr_bal)) {
            printf("Balance for address %s: %lf\n", argv[i], bal);
        } else {
            printf("Address %s: not found\n", argv[i]);
        }
    }  
    blockchain_free(bc);
}
