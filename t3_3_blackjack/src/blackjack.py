#! /usr/bin/env python
import random

# A hand is represented as a pair (total, ace) where:
#  - total is the point total of cards in the hand (counting aces as 1)
#  - ace is true if the hand contains an ace

global deck

def reset_deck():
    global deck
    deck = []
    for _ in range(4):
        # Ace to 10
        for j in range(1, 11):
            deck.extend([j])
        # Jack, queen, king
        for _ in range(3):
            deck.extend([10])

# Return the empty hand.
def empty_hand():
    return (0, False)

# Return whether the hand has a usable ace.
# Note that a hand may contain an ace, but it might not be usable.
def hand_has_usable_ace(hand):
    total, ace = hand
    return ((ace) and ((total + 10) <= 21))

# Return the value of the hand.
# The value of the hand is either total or total + 10 (if the ace is usable)
def hand_value(hand):
    total, _ = hand
    if (hand_has_usable_ace(hand)):
        return (total + 10)
    else:
        return total

# Update a hand by adding a card to it.
# Return the new hand.
def hand_add_card(hand, card):
    total, ace = hand
    total = total + card
    if (card == 1):
        ace = True
    return (total, ace)

# Return the reward of the game (-10 or 10) given the final player and dealer
# hands.
def game_reward(player_hand, dealer_hand):
    player_val = hand_value(player_hand)
    dealer_val = hand_value(dealer_hand)
    if (player_val > 21):
        return -10.0
    elif (dealer_val > 21):
        return 10.0
    # On equally valued hands, the player looses
    elif (player_val <= dealer_val):
        return -10.0
    elif (player_val > dealer_val):
        return 10.0

# Draw a card from the deck
# Return the face value of the card (1 to 10).
def draw_card():
    global deck
    card_ind = random.randint(0, len(deck) - 1)
    card = deck[card_ind]
    del deck[card_ind]
    return card

# Deal a player hand given the function to draw cards.
# Return only the hand.
def deal_player_hand():
    hand = empty_hand()
    hand = hand_add_card(hand, draw_card())
    hand = hand_add_card(hand, draw_card())
    while (hand_value(hand) < 11):
        hand = hand_add_card(hand, draw_card())
    return hand

# Deal the first card of a dealer hand given the function to draw cards.
# Return the hand and the shown card.
def deal_dealer_hand():
    hand = empty_hand()
    card = draw_card()
    hand = hand_add_card(hand, card)
    return hand, card

# Play the dealer hand using the fixed strategy for the dealer.
# Return the resulting dealer hand.
def play_dealer_hand(hand):
    while (hand_value(hand) < 17):
        hand = hand_add_card(hand, draw_card())
    return hand

# States are tuples (card, value, usable) where
#  - card is the card the dealer is showing
#  - value is the current value of the player's hand
#  - usable is whether or not the player has a usable ace

# Actions are either stay (False) or hit (True)

# Select a state at random.
def select_random_state(all_states):
    reset_deck()
    n = len(all_states)
    r = random.randint(0, n - 1)
    state = all_states[r]
    return state

# Select an action at random
def select_random_action():
    r = random.randint(1, 2)
    return (r == 1)

# Select the best action using current Q-values.
def select_best_action(Q, state):
    if (Q[(state, True)] > Q[(state, False)]):
        return True
    else:
        return False

# Select an action according to the epsilon-greedy policy
def select_e_greedy_action(Q, state, epsilon):
    r = random.random()
    if (r < epsilon):
        return select_random_action()
    else:
        return select_best_action(Q, state)

def remove_cards_from_deck(card, val, usable):
    global deck
    deck.remove(card)
    aux_val = val
    # If usable make sure we remove a 1
    if usable:
        deck.remove(1)
        aux_val = aux_val - 1
    # Remove a non 10 card
    if aux_val % 10 != 0: 
        deck.remove(aux_val % 10)
    # Remove any remaining 10 cards until val is reached
    while aux_val >= 10:
        deck.remove(10)
        aux_val = aux_val - 10

# Given the state, return player and dealer hand consistent with it.
def hands_from_state(state):
    card, val, usable = state
    if (usable):
        val = val - 10
    player_hand = (val, usable)
    dealer_hand = empty_hand()
    dealer_hand = hand_add_card(dealer_hand, card)
    remove_cards_from_deck(card, val, usable)
    return card, dealer_hand, player_hand

# Given the dealer's card and player's hand, return the state.
def state_from_hands(card, player_hand):
    val = hand_value(player_hand)
    usable = hand_has_usable_ace(player_hand)
    return (card, val, usable)

# Return a list of the possible states.
def state_list():
    states = []
    for card in range(1, 11):
        for val in range(11, 22):
            states.append((card, val, False))
            states.append((card, val, True))
    return states

# Return a map of all (state, action) pairs -> values (initially zero)
def initialize_state_action_value_map():
    states = state_list()
    M = {}
    for state in states:
        M[(state, False)] = 0.0
        M[(state, True)] = 0.0
    return M

# Print a (state, action) -> value map
def print_state_action_value_map(Q):
    for usable in [True, False]:
        if usable:
            print 'Usable ace'
        else:
            print 'Non usable ace'
        print 'Values for staying:'
        for val in range(21, 10, -1):
            for card in range(1, 11):
                print '%5.2f' % Q[((card, val, usable), False)], ' ',
            print '| %d' % val
        print 'Values for hitting:'
        for val in range(21, 10, -1):
            for card in range(1, 11):
                print '%5.2f' % Q[((card, val, usable), True)], ' ',
            print '| %d' % val
        print ' '

# Print the state-action-value function (Q)
def print_Q(Q):
    print '---- Q(s,a) ----'
    print_state_action_value_map(Q)

# Print the state-value function (V) given the Q-values
def print_V(Q):
    print '---- V(s) ----'
    for usable in [True, False]:
        if usable:
            print 'Usable ace'
        else:
            print 'No usable ace'
        for val in range(21, 10, -1):
            for card in range(1, 11):
                if (Q[((card, val, usable), True)] > Q[((card, val, usable), False)]):
                    print '%5.2f' % Q[((card, val, usable), True)], ' ',
                else:
                    print '%5.2f' % Q[((card, val, usable), False)], ' ',
            print '| %d' % val
        print ' '

# Print a policy given the Q-values
def print_policy(Q):
    print '---- Policy ----'
    for usable in [True, False]:
        if usable:
            print 'Usable ace'
        else:
            print 'No usable ace'
        for val in range(21, 10, -1):
            for card in range(1, 11):
                if (Q[((card, val, usable), True)] > Q[((card, val, usable), False)]):
                    print 'X',
                else:
                    print ' ',
            print '| %d' % val
        print ' '

# Initialise Q-values so that they produce the initial policy of sticking
# only on 20 or 21.
def initialize_Q():
    states = state_list()
    M = {}
    for state in states:
        _, val, _ = state
        if (val < 20):
            M[(state, False)] = -0.001
            M[(state, True)] = 0.001  # favour hitting
        else:
            M[(state, False)] = 0.001  # favour sticking
            M[(state, True)] = -0.001
    return M

# Q-learning.
#
# Run Q-learning for the specified number of iterations and return the Q-values.
def q_learning(n_iter, alpha, epsilon):
    # initialise Q and count
    Q = initialize_Q()
    count = initialize_state_action_value_map()
    # get list of all states
    all_states = state_list()
    # iterate
    for _ in range(n_iter):
        # initialise state
        state = select_random_state(all_states)
        dealer_card, dealer_hand, player_hand = hands_from_state(state)
        keep_playing = True
        # choose actions, update Q
        while (keep_playing):
            action = select_e_greedy_action(Q, state, epsilon)
            sa = (state, action)
            if (action):
                # draw a card, update state
                player_hand = hand_add_card(player_hand, draw_card())
                # check if busted
                if (hand_value(player_hand) > 21):
                    # update Q-value
                    count[sa] = count[sa] + 1.0
                    Q[sa] = Q[sa] + alpha / count[sa] * ((-1.0) - Q[sa])
                    keep_playing = False
                else:
                    # update Q-value
                    s_next = state_from_hands(dealer_card, player_hand)
                    q_best = Q[(s_next, False)]
                    if (Q[(s_next, True)] > q_best):
                        q_best = Q[(s_next, True)]
                    count[sa] = count[sa] + 1.0
                    Q[sa] = Q[sa] + alpha / count[sa] * (q_best - Q[sa])
                    # update state
                    state = s_next
            else:
                # let the dealer play
                dealer_hand = play_dealer_hand(dealer_hand)
                # compute return
                R = game_reward(player_hand, dealer_hand)
                # update Q value
                count[sa] = count[sa] + 1.0
                Q[sa] = Q[sa] + alpha / count[sa] * (R - Q[sa])
                keep_playing = False
    return Q

# Compute the expected value of the game using the learned Q-values.
def expected_gain(Q, n_iter):
    gain = 0.0
    for _ in range(n_iter):
        reset_deck()
        player_hand = deal_player_hand()
        _, dealer_card = deal_dealer_hand()
        state = state_from_hands(dealer_card, player_hand)
        v = Q[(state, False)]
        if (Q[(state, True)] > v):
            v = Q[(state, True)]
        gain = gain + v
    print 'Expected gain: %6.3f' % (gain / float(n_iter))
    print ' '

# Main program
if __name__ == '__main__':
    # set parameters
    n_iter_mc = 10000
    n_iter_q = 10000
    n_games = 1000
    alpha = 1
    epsilon = 0.1
    print 'Q-LEARNING'
    Q = q_learning(n_iter_q, alpha, epsilon)
    print_Q(Q)
    print_V(Q)
    print_policy(Q)
    expected_gain(Q, n_games)
    exit(0)    
