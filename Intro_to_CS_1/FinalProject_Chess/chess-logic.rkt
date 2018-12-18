#lang typed/racket

;; Reference solution to Project 1, CMSC 15100, Univ. of Chicago, Autumn 2017
;; Theis code is written by Adam Shaw and Matthew Wachs.
;; You may use all of the code or any part of the code in Project 2.
;; If you do use our code, attribute clearly.

;; You may not share this file with anyone, now or in the future.
;; It is for individual use only. You are on your honor.

(require typed/test-engine/racket-tests)

(require "../include/cs151-core.rkt")
(require "../include/cs151-image.rkt")

(require "../project1/optional.rkt")
(require "../project1/loc.rkt")

;; ==== ==== ==== ====
;; external interface

(provide PieceType
         Player
         (struct-out Piece)
         Square
         Board
         (struct-out Move)
         PromoteTo
         (struct-out ChessGame)
         starting-board ; : Board
         new-game       ; : ChessGame
         board-ref      ; : Board Loc -> Square
         board-update   ; : Board Loc Square -> Board
         in-check?      ; : ChessGame -> Boolean
         legal-move?    ; : ChessGame Move -> Boolean
         moves-piece    ; : ChessGame Loc -> (Listof Move)
         moves-player   ; : ChessGame -> (Listof Move)
         checkmate?     ; : ChessGame -> Boolean
         stalemate?     ; : ChessGame -> Boolean
         apply-move     ; : ChessGame Move -> ChessGame
         strings->board ; : (Listof String) -> Board
         rank->int
         int->rank
         file->int
         int->file
         occ?
         take
         drop
         en-passant?
         sub1-file
         add1-file
         sub1-rank
         add1-rank
         )

;; ==== ==== ==== ====
;; data definitions

(define-type PieceType
  (U 'Pawn 'Bishop 'Knight 'Rook 'King 'Queen))

(define-type Player
  (U 'Black 'White))

(define-struct Piece
  ([type  : PieceType]
   [color : Player]))

(define-type Square
  (Optional Piece))

(define-type Board
  (Listof Square))

(define-type PromoteTo
  (U 'Queen 'Rook 'Bishop 'Knight))

(define-struct Move
  ([src        : Loc]
   [dst        : Loc]
   [moved      : Piece]
   [captured   : (Optional Piece)]
   [promote-to : (Optional PromoteTo)]))

(define-struct ChessGame
  ([board : Board]
   [history : (Listof Move)]))

;; ==== ==== ==== ====
;; supplemental data definitions

;; The Vec data structure is used to enact moves to other locations.
;; In some cases it represents a direction, like (Vec 1 1) for "northeast";
;; in other cases it represents a displacement, like (Vec 2 1) for a knight.

(define-struct Vec
  ([delta-rank : (U -2 -1 0 1 2)]
   [delta-file : (U -2 -1 0 1 2)]))

;; ==== ==== ==== ====
;; === option operations
;; NOTE it might make sense to put these in optional.rkt

(: none? (All (T) ((Optional T) -> Boolean)))
;; test if an option is 'None
(define (none? opt)
  (match opt ['None #t] [_ #f]))

(: some? (All (T) ((Optional T) -> Boolean)))
;; test if an option is Some
(define (some? opt)
  (not (none? opt)))

(: opt=? (All (T) ((T T -> Boolean) (Optional T) (Optional T) -> Boolean)))
;; equality test, with option wrapper taken into account
(define (opt=? eq opt1 opt2)
  (match* (opt1 opt2)
    [('None 'None) #t]
    [((Some a) (Some b)) (eq a b)]
    [(_ _) #f]))
   
;; === general purpose utilities

(: find : All (A B) (A A -> Boolean) A (Listof (List A B)) -> (Optional B))
;; find value associated with key, if there is one
;; note: List is here used as a pair constructor and is distinct from Listof
(define (find eq key finite-map)
  (local
    {(: fnd : (Listof (List A B)) -> (Optional B))
     (define (fnd pairs)
       (match pairs
         ['() 'None]
         [(cons (list k v) rest) (if (eq k key) (Some v) (fnd rest))]))}
    (fnd finite-map)))

(: list-update (All (A) ((Listof A) Integer A -> (Listof A))))
;; functional list update by position
;; ex: (list-update '(A B C D) 2 'X) ==> '(A B X D)
(define (list-update xs i x)
  (if (zero? i)
      (cons x (rest xs))
      (cons (first xs) (list-update (rest xs) (sub1 i) x))))

(: position-of : All (A) (A A -> Boolean) A (Listof A) -> (Optional Integer))
;; find the position of first occurence of item in list, if present 
(define (position-of eq x items)
  (local
    {(: pos : (Listof A) Integer -> (Optional Integer))
     (define (pos xs i)
       (match xs
         ['() 'None]
         [(cons first rest)
          (if (eq first x) (Some i) (pos rest (add1 i)))]))}
    (pos items 0)))

(: index-of : All (A) (A -> Boolean) (Listof A) -> (Optional Integer))
;; find position of first item to pass the test in list, if there is one
;; note: position-of is for equality, index-of is generalized to any property
(define (index-of f xs)
  (local
    {(: index : (Listof A) Integer -> (Optional Integer))
     (define (index xs i)
       (match xs
         ['() 'None]
         [(cons first rest)
          (if (f first) (Some i) (index rest (add1 i)))]))}
    (index xs 0)))

(: map-append : All (A B) (A -> (Listof B)) (Listof A) -> (Listof B))
;; gather a list of list results into one list
;;   - like map, but using append in place of cons
;; ex: (map-append countdown '(3 2 3)) ==> '(3 2 1 2 1 3 2 1)
;;   - assuming (countdown 3) ==> '(3 2 1) etc.
;; cf: (map countdown '(3 2 3)) ==> '((3 2 1) (2 1) (3 2 1))
(define (map-append f xs)
  (match xs
    ['() '()]
    [(cons first rest) (append (f first) (map-append f rest))]))

;; === board construction from strings

;; a mapping of characters to piece types
(define piece-map
  (list (list #\p 'Pawn)
        (list #\r 'Rook)
        (list #\b 'Bishop)
        (list #\n 'Knight)
        (list #\k 'King)
        (list #\q 'Queen)))

(: char->piece : Char -> Piece)
(define (char->piece c)
  (match (find char=? (char-downcase c) piece-map)
    ['None (error (string-append "no such piece: " (string c)))]
    [(Some type) (Piece type (if (char-upper-case? c) 'Black 'White))]))

(: char->square : Char -> Square)
(define (char->square c)
  (match c
    [#\- 'None]
    [_ (Some (char->piece c))]))

(: strings->board : (Listof String) -> Board)
;; turn a list of eight strings into a board
;; see starting-board below for an example
(define (strings->board ss)
  (local
    {(: string->squares : String -> (Listof Square))
     (define (string->squares s)
       (map char->square (string->list s)))}
  (map-append string->squares (reverse ss))))

;; === new chess game

(: starting-board : Board)
(define starting-board
  (strings->board (list "RNBQKBNR"
                        "PPPPPPPP"
                        "--------"
                        "--------"
                        "--------"
                        "--------"
                        "pppppppp"
                        "rnbqkbnr")))

(: new-game : ChessGame)
(define new-game
  (ChessGame starting-board '()))

;; === basic chess operations

(: rank->int : Rank -> Integer)
;; convert a rank on [1,8] to an Integer on [0,7]
(define (rank->int r)
  (sub1 r))

(: int->rank : Integer -> Rank)
;; convert an Integer on [0-7] to a rank [1,8]
(define (int->rank i)
  (cast (add1 i) Rank))

(: file->int : File -> Integer)
;; convert a file, from 'A-'H, to an Integer [0,7]
(define (file->int f)
  (val-of (position-of symbol=? f '(A B C D E F G H))))

(: int->file : Integer -> File)
;; convert an Integer on [0,7] to a file ['A,'H]
(define (int->file i)
  (list-ref '(A B C D E F G H) i))

(: add1-rank : Rank -> (Optional Rank))
;; increment a rank, return 'None if off the board
(define (add1-rank r)
  (if (< r 8) (Some (cast (add1 r) Rank)) 'None))

(: sub1-rank : Rank -> (Optional Rank))
;; decrement a rank, return 'None if off the board
(define (sub1-rank r)
  (if (> r 1) (Some (cast (sub1 r) Rank)) 'None))

(: add-rank : Rank Integer -> (Optional Rank))
;; compute a new rank, return 'None if off the board
(define (add-rank r d)
  (if (<= 1 (+ r d) 8)
      (Some (cast (+ r d) Rank))
      'None))

(: add1-file : File -> (Optional File))
;; increment a file, return 'None if off the board
(define (add1-file f)
  (match f
    ['A (Some 'B)]
    ['B (Some 'C)]
    ['C (Some 'D)]
    ['D (Some 'E)]
    ['E (Some 'F)]
    ['F (Some 'G)]
    ['G (Some 'H)]
    ['H 'None]))

(: sub1-file : File -> (Optional File))
;; decrement a file, return 'None if off the board
(define (sub1-file f)
  (match f
    ['A 'None]
    ['B (Some 'A)]
    ['C (Some 'B)]
    ['D (Some 'C)]
    ['E (Some 'D)]
    ['F (Some 'E)]
    ['G (Some 'F)]
    ['H (Some 'G)]))

(: add-file : File Integer -> (Optional File))
;; compute a new file, return 'None if off the board
(define (add-file f d)
  (cond
    [(zero? d) (Some f)]
    [(positive? d)
      (match (add1-file f)
        ['None 'None]
        [(Some f+1) (add-file f+1 (sub1 d))])]
    [else
      (match (sub1-file f)
        ['None 'None]
        [(Some f-1) (add-file f-1 (add1 d))])]))

(: add-loc : Loc Integer Integer -> (Optional Loc))
;; compute Some new location by adding to rank and file,
;; return 'None if off the board
(define (add-loc loc delta-rank delta-file)
  (match loc
    [(Loc file rank)
      (match (add-rank rank delta-rank)
        ['None 'None]
        [(Some rank~)
          (match (add-file file delta-file)
            ['None 'None]
            [(Some file~) (Some (Loc file~ rank~))])])]))

(: int->loc : Integer -> Loc)
;; 0 => A1, 1=>B1, ..., 8=>A2, ..., 63=>H8
(define (int->loc i)
  (if (or (< i 0) (> i 63))
      (error "int->loc: out of range")
      (Loc (int->file (remainder i 8))
           (int->rank (quotient i 8)))))

(: every-loc : (Listof Loc))
(define every-loc
  (build-list 64 int->loc))

(: loc->index : Loc -> Integer)
;; convert a Loc to a list index in the 1-D board representation
(define (loc->index loc)
  (match loc
    [(Loc file rank)
      (+ (* (rank->int rank) 8) (file->int file))]))

(: board-ref : Board Loc -> Square)
;; return the (Optional Piece) at the given location
(define (board-ref b loc)
  (list-ref b (loc->index loc)))

(: piece-at : Board Loc -> Piece)
;; similar to board ref, but return the piece or raise error
(define (piece-at b loc)
  (match (board-ref b loc)
    [(Some p) p]
    ['None (error "empty square")]))

(: unocc? : Board Loc -> Boolean)
;; return true if the location is unoccupied
(define (unocc? b loc)
  (none? (board-ref b loc)))

(: occ? : Board Loc -> Boolean)
;; return true if the location is occupied
(define (occ? b loc)
  (some? (board-ref b loc)))
             
(: board-update : Board Loc Square -> Board)
;; update the board location
(define (board-update b loc s)
  (list-update b (loc->index loc) s))

(: take : All (A) Integer (Listof A) -> (Listof A))
;;build new list with n items from a list
(define (take n xs)
  (match xs
    ['() '()]
    [(cons first rest)
     (cond
       [(= n 0) '()]
       [(> n 0) (cons first (take (sub1 n) rest))]
       [else (error "take: must request a positive number of items")])]))

(check-expect (take 8 starting-board) (list (Some (Piece 'Rook 'White)) (Some (Piece 'Knight 'White))
                                            (Some (Piece 'Bishop 'White)) (Some (Piece 'Queen 'White))
                                            (Some (Piece 'King 'White)) (Some (Piece 'Bishop 'White))
                                            (Some (Piece 'Knight 'White)) (Some (Piece 'Rook 'White))))

(: drop : All (A) Integer (Listof A) -> (Listof A))
;;returns a list without the first n items
(define (drop n xs)
  (match xs
    ['() '()]
    [(cons first rest) (cond
                         [(= n 0) rest]
                         [(> n 0) (drop (sub1 n) rest)]
                         [else (error "drop: input must be non-negative integer")])]))

(check-expect (drop 55 starting-board) (list    (Some (Piece 'Rook 'Black))  (Some (Piece 'Knight 'Black))
                                                (Some (Piece 'Bishop 'Black))  (Some (Piece 'Queen 'Black))
                                                (Some (Piece 'King 'Black))  (Some (Piece 'Bishop 'Black))
                                                (Some (Piece 'Knight 'Black)) (Some (Piece 'Rook 'Black))))

;; === general-purpose chess and chess move widgets

(: piece-type=? : PieceType PieceType -> Boolean)
(define (piece-type=? p1 p2)
  (symbol=? p1 p2))

(: player=? : Player Player -> Boolean)
(define (player=? p1 p2)
  (symbol=? p1 p2))

(: piece=? : Piece Piece -> Boolean)
(define (piece=? p1 p2)
  (and (symbol=? (Piece-type p1) (Piece-type p2))
       (symbol=? (Piece-color p1) (Piece-color p2))))

(: move=? (Move Move -> Boolean))
;; equality tests for moves
(define (move=? m1 m2)
  (match* (m1 m2)
    [((Move s1 d1 m1 c1 p1) (Move s2 d2 m2 c2 p2))
     (and (loc=? s1 s2)
          (loc=? d1 d2)
          (piece=? m1 m2)
          (opt=? piece=? c1 c2)
          (opt=? symbol=? p1 p2))]))

(: opponent : Player -> Player)
;; return the other player
(define (opponent p)
  (match p
    ['Black 'White]
    ['White 'Black]))

(: opponents? : Board Loc Loc -> Boolean)
;; are the pieces on the squares opponents?
(define (opponents? b loc1 loc2)
  (match* ((board-ref b loc1) (board-ref b loc2))
    [((Some (Piece _  color1)) (Some (Piece _ color2)))
     (player=? color2 (opponent color1))]
    [(_ _) (error "one or both locations empty")]))

(: move-if-unocc : Board Loc Integer Integer -> (Listof Move))
;; given a board, a location, a delta-rank and a delta-file,
;; if the new location is unoccupied, return it in a singleton list
;; NOTE: this returns either empty or a list of length 1
;; -- this convention is for use in various calls to append below
(define (move-if-unocc b src delta-rank delta-file)
  (match (add-loc src delta-rank delta-file)
    ['None '()]
    [(Some dst)
     (match (board-ref b dst)
       ['None (list (Move src dst (piece-at b src) 'None 'None))]
       [(Some _) '()])]))

(: capture-if-occ : Board Loc Integer Integer -> (Listof Move))
;; given a board, a location, a delta-rank and a delta-file,
;; if the new location is unoccupied by an opponent's piece,
;; return it in a singleton list
;; NOTE: this returns either empty or a list of length 1
;; -- this convention is for use in various calls to append below
(define (capture-if-occ b loc delta-rank delta-file)
  (match (add-loc loc delta-rank delta-file)
    ['None '()]
    [(Some loc~)
     (if (and (occ? b loc~) (opponents? b loc loc~))
         (list (Move loc
                     loc~
                     (val-of (board-ref b loc))
                     (board-ref b loc~)
                     'None))
         '())]))

(: move-if-possible : Board Loc Vec -> (Listof Move))
;; move to given other location, directly, with capture if possible
;; (this is useful for kings and knights)
;; NOTE: this returns either empty or a list of length 1
;; -- this convention is for use in various calls to append below
(define (move-if-possible b loc vec)
  (match vec
    [(Vec delta-rank delta-file)
     (local
       {(: mv : Loc -> (Listof Move))
        ;; construct a Move for the moving piece
        (define (mv dst)
          (list (Move loc dst (piece-at b loc) (board-ref b dst) 'None)))}
       (match (add-loc loc delta-rank delta-file)
         ['None '()]
         [(Some loc~)
          (cond
            [(and (occ? b loc~) (opponents? b loc loc~)) (mv loc~)]
            [(unocc? b loc~) (mv loc~)]
            [else '()])]))]))

(: explore (Board Loc Vec -> (Listof Move)))
;; explore in the given direction until the moving piece
;; either runs into a teammate, captures an opponent, or
;; walks off the board
(define (explore b loc vec)
  (match vec
    [(Vec delta-rank delta-file)
     (if (and (zero? delta-rank) (zero? delta-file))
         (error "explore: infinite loop")
         (local
           {(: mv : Loc -> Move)
            ;; construct a move to open location
            (define (mv dst)
              (Move loc dst (piece-at b loc) 'None 'None))
            (: capture : Loc -> Move)
            ;; construct a capture
            (define (capture dst)
              (Move loc dst (piece-at b loc) (board-ref b dst) 'None))
            (: lp : Loc -> (Listof Move))
            (define (lp curr)
              (match (add-loc curr delta-rank delta-file)
                ['None '()]
                [(Some loc~)
                 (cond
                   [(occ? b loc~) (if (opponents? b loc loc~)
                                      (list (capture loc~))
                                      '())]
                   [(unocc? b loc~) (cons (mv loc~) (lp loc~))]
                   [else (error "this branch unreachable")])]))}
           (lp loc)))]))

(define rook-vecs
  (list (Vec 1 0) (Vec 0 1) (Vec -1 0) (Vec 0 -1)))

(define bishop-vecs
  (list (Vec 1 1) (Vec -1 1) (Vec -1 -1) (Vec 1 -1)))

(define knight-vecs
;; in the case of knights, these are not vectors but displacements
  (list (Vec -2 -1) (Vec -2 1)
        (Vec -1 -2) (Vec -1 2)
        (Vec  1 -2) (Vec  1 2)
        (Vec  2 -1) (Vec  2 1)))

(define queen-vecs
  (append rook-vecs bishop-vecs))

(: piece-at? : Board Piece (Optional Loc) -> Boolean)
;; check if there is a particular piece at the given loc
(define (piece-at? b pc opt-loc)
  (match opt-loc
    ['None #f]
    [(Some loc)
     (match (board-ref b loc)
       [(Some pc~) (piece=? pc pc~)]
       [_ #f])]))

(: piece-along? : Board Loc Piece Vec -> Boolean)
;; move along the board until finding a piece or not
(define (piece-along? b loc pc vec)
  (match vec
    [(Vec delta-rank delta-file)
     (local
       {(: lp : Loc -> Boolean)
        (define (lp loc)
          (match (add-loc loc delta-rank delta-file)
            ['None #f]
            [(Some loc~) 
             (match (board-ref b loc~)
               ['None (lp loc~)]
               [(Some pc~) (piece=? pc pc~)])]))}
       (lp loc))]))
  
;; === move logic for the different pieces

(: moves-king (Board Loc (Listof Move) -> (Listof Move)))
;; return the list of moves available to a king
;; NOTE: this doesn't account for check
(define (moves-king b loc moves)
  (match (board-ref b loc)
    [(Some (Piece (not 'King) _)) (error "not king")]
    ['None (error "empty square")]
    [_ (append
        (castling b loc moves)
        (map-append (λ ([v : Vec]) (move-if-possible b loc v))
                   queen-vecs))]))

(: moves-knight (Board Loc (Listof Move) -> (Listof Move)))
;; return the list of moves available to a knight
;; NOTE: this doesn't account for check
(define (moves-knight b loc moves)
  (match (board-ref b loc)
    [(Some (Piece (not 'Knight) _)) (error "not a knight")]
    ['None (error "location unoccupied")]
    [(Some (Piece 'Knight player))
     (map-append (λ ([v : Vec]) (move-if-possible b loc v))
                 knight-vecs)]))
            
(: moves-pawn (Board Loc (Listof Move) -> (Listof Move)))
;; return the list of moves available to a pawn
;; NOTE: this doesn't account for whether this exposes check
(define (moves-pawn b loc moves)
  (match (board-ref b loc)
    [(Some (Piece (not 'Pawn) _)) (error "not a pawn")]
    ['None (error "location unoccupied")]
    [(Some (Piece 'Pawn player))
     (local
       {(define delta-rank
          (match player ['Black -1] ['White 1]))}
       (match loc
         [(Loc _ '1) (error "pawn cannot be in rank 1")]
         [(Loc _ '8) (error "pawn cannot be in rank 8")]
         [(Loc _ rank)
          (append (capture-if-occ b loc delta-rank -1)
                  (move-if-unocc b loc delta-rank 0)
                  (if (and (= rank (match player ['Black '7] ['White '2]))
                           (unocc? b (val-of (add-loc loc delta-rank 0))))
                      (move-if-unocc b loc (* 2 delta-rank) 0)
                      '())
                  (en-passant b loc player moves)
                  (capture-if-occ b loc delta-rank 1))]))]))

(: en-passant (Board Loc Player (Listof Move) -> (Listof Move)))
;;enumerates en passant moves
(define (en-passant b loc player hist)
  (match hist
    ['() '()]
    [(cons (Move src dst (Piece type p) _ _) _)
     (if (and (symbol=? type 'Pawn)
              (= (Loc-rank loc) (Loc-rank dst))
              (= (abs (- (rank->int (Loc-rank src)) (rank->int (Loc-rank dst)))) 2)
              (= (abs (- (file->int (Loc-file dst)) (file->int (Loc-file loc)))) 1)
              (not (symbol=? player p)))
              (list (match player
                ['Black (Move loc (Loc (Loc-file dst) (val-of (sub1-rank (Loc-rank dst))))
                              (val-of (board-ref b loc)) (board-ref b dst) 'None)]     
                ['White (Move loc (Loc (Loc-file dst) (val-of (add1-rank (Loc-rank dst))))
                              (val-of (board-ref b loc)) (board-ref b dst) 'None)]))
              '())]))
    

(check-expect (en-passant (strings->board (list "--------"
                                                "--------"
                                                "--------"
                                                "----Pp--"
                                                "--------"
                                                "--------"
                                                "--------"
                                                "--------"))
                          (Loc 'F 5) 'White (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'Black)
                                                        'None 'None)))
              (list (Move (Loc 'F 5) (Loc 'E 6) (Piece 'Pawn 'White)
                          (Some (Piece 'Pawn 'Black)) 'None)))

(check-expect (en-passant (strings->board (list "--------"
                                                "--------"
                                                "--------"
                                                "---pP--"
                                                "--------"
                                                "--------"
                                                "--------"
                                                "--------"))
                          (Loc 'D 5) 'White (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'Black)
                                                        'None 'None)))
              (list (Move (Loc 'D 5) (Loc 'E 6) (Piece 'Pawn 'White)
                          (Some (Piece 'Pawn 'Black)) 'None)))

(check-expect (en-passant (strings->board (list "--------"
                                                "--------"
                                                "--------"
                                                "----P--"
                                                "----p---"
                                                "--------"
                                                "--------"
                                                "--------"))
                          (Loc 'E 4) 'White (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'Black)
                                                        'None 'None)))
              '())

(check-expect (en-passant (strings->board (list "--------"
                                                "--------"
                                                "--------"
                                                "----Pp--"
                                                "--------"
                                                "--------"
                                                "--------"
                                                "--------"))
                          (Loc 'F 5) 'White (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Knight 'Black)
                                                        'None 'None)))
              '())

(check-expect (en-passant (strings->board (list "--------"
                                                "--------"
                                                "--------"
                                                "----Pp--"
                                                "--------"
                                                "--------"
                                                "--------"
                                                "--------"))
                          (Loc 'F 5) 'White (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'White)
                                                        'None 'None)))
             '())

(check-expect (en-passant (strings->board (list "--------"
                                                "--------"
                                                "--------"
                                                "--------"
                                                "----Pp--"
                                                "--------"
                                                "--------"
                                                "--------"))
                          (Loc 'E 4) 'Black (list (Move (Loc 'F 2) (Loc 'F 4)
                                                        (Piece 'Pawn 'White)
                                                        'None 'None)))
              (list (Move (Loc 'E 4) (Loc 'F 3) (Piece 'Pawn 'Black)
                          (Some (Piece 'Pawn 'White)) 'None)))

(: castle-options : Board Player -> (Listof File))
;;checks if rooks are in the proper place
;;NOTE: does not account for in check
(define (castle-options b turn)
  (match turn
    ['Black (match (board-ref b (Loc 'E 8))
              [(Some (Piece 'King 'Black)) (append
                                            (match (board-ref b (Loc 'A 8))
                                              [(Some (Piece 'Rook 'Black)) (list 'A)]
                                              [_ '()])
                                            (match (board-ref b (Loc 'H 8))
                                              [(Some (Piece 'Rook 'Black)) (list 'H)]
                                              [_ '()]))]
              [_ '()])]
                
    ['White (match (board-ref b (Loc 'E 1))
              [(Some (Piece 'King 'White)) (append
                                            (match (board-ref b (Loc 'A 1))
                                              [(Some (Piece 'Rook 'White)) (list 'A)]
                                              [_ '()])
                                            (match (board-ref b (Loc 'H 1))
                                              [(Some (Piece 'Rook 'White)) (list 'H)]
                                              [_ '()]))]
              [_ '()])]))

(check-expect (castle-options (strings->board (list "R---K--R"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) (list 'A 'H))

(check-expect (castle-options (strings->board (list "R---K--N"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) (list 'A))

(check-expect (castle-options (strings->board (list "R---K--r"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) (list 'A))

(check-expect (castle-options (strings->board (list "----K--R"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) (list 'H))

(check-expect (castle-options (strings->board (list "R---K---"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) (list 'A))

(check-expect (castle-options (strings->board (list "R---k--R"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) '())

(check-expect (castle-options (strings->board (list "R------N"
                                                     "----K---"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"))
                               'Black) '())

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "r---k--r"))
                               'White) (list 'A 'H))

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "r---k--r"))
                               'Black) '())

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "r---k---"))
                               'White) (list 'A))

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "----k---"))
                               'White) '())

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "n---k--b"))
                               'White) '())

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "b---k--r"))
                               'White) (list 'H))

(check-expect (castle-options (strings->board (list "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "--------"
                                                     "R---k--r"))
                               'White) (list 'H))


(: lamb : Move (Listof File) Player -> (Listof File))
(define (lamb move poss player)
  (match poss
    ['() '()]
    [(cons first rest) (match move
                         [(Move (Loc srcf srcr) dst (Piece t~ p~) cap _)
                          (if (symbol=? p~ player)
                              (match t~
                                ['King '()]
                                ['Rook (match srcf
                                         ['A rest]
                                         ['H (list first)]
                                         [_ poss])])
                              poss)])]))

(: custom-fold : (Listof Move) (Listof File) Player 
   (Move (Listof File) Player -> (Listof File)) -> (Listof File))
(define (custom-fold moves acc p l)
  (match moves
    ['() acc]
    [(cons first rest) (custom-fold rest (l first acc p) p l)]))
                                           
(: castling (Board Loc (Listof Move) -> (Listof Move)))
;;lists possible castling moves
(define (castling b loc hist)
  (local
     {(: turn : Player)
      (define turn (whose-turn (ChessGame b hist)))
      (: lamb : Move (Listof File) Player -> (Listof File))
     (define (lamb move poss player)
       (match poss
         ['() '()]
         [(cons first rest) (match move
                              [(Move (Loc srcf srcr) dst (Piece t~ p~) cap _)
                               (if (symbol=? p~ player)
                                   (match t~
                                     ['King '()]
                                     ['Rook (match srcf
                                              ['A rest]
                                              ['H (list first)]
                                              [_ poss])]
                                     [_ poss])
                                   poss)])]))
     (: custom-fold : (Listof Move) (Listof File) Player 
        (Move (Listof File) Player -> (Listof File)) -> (Listof File))
     (define (custom-fold moves acc p l)
       (match moves
         ['() acc]
         [(cons first rest) (custom-fold rest (l first acc p) p l)]))}
    
    (if (not (in-check? (ChessGame b hist)))
        (match (custom-fold hist (castle-options b turn) turn lamb)
          ['() '()]
          [(cons first rest) (match* (first rest)
                               [('H '()) (castle-right b loc turn hist)]
                               [('A '()) (castle-left b loc turn hist)]
                               [(_ _) (append
                                       (castle-left b loc turn hist)
                                       (castle-right b loc turn hist))])])                                 
        '())))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "r---k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)
               (Move (Loc 'E 1) (Loc 'G 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'H 1) (Loc 'F 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "----k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'G 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'H 1) (Loc 'F 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "r-n-k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'G 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'H 1) (Loc 'F 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--R-----"
                                              "--------"
                                              "r---k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'G 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'H 1) (Loc 'F 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "---R----"
                                              "--------"
                                              "r---k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'G 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'H 1) (Loc 'F 1) (Piece 'Rook 'White) 'None 'None)))


(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "rb--k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'G 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'H 1) (Loc 'F 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "r---k---"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "r---k-nr"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "r---kb-r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "------R-"
                                              "--------"
                                              "r---k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)))

(check-expect (castling (strings->board (list "---K----"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "-----R--"
                                              "--------"
                                              "r---k--r"))
                        (Loc 'E 1) '())
              (list
               (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
               (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)))


(check-expect (castling (strings->board (list "R---K--R"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "----k---"))
                        (Loc 'E 8) (list (Move (Loc 'A 2) (Loc 'A 3) (Piece 'Bishop 'Black) 'None 'None)))
              '())

(check-expect (castling (strings->board (list "R---K--R"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "-----r--"
                                              "--------"
                                              "r---k--r"))
                        (Loc 'E 8) (list (Move (Loc 'A 2) (Loc 'A 3) (Piece 'Bishop 'White) 'None 'None)))
              (list
               (Move (Loc 'E 8) (Loc 'C 8) (Piece 'King 'Black) 'None 'None)
               (Move (Loc 'A 8) (Loc 'D 8) (Piece 'Rook 'Black) 'None 'None)))





(: castle-right (Board Loc Player (Listof Move) -> (Listof Move)))
;;determines if the king can castle rightwards
(define (castle-right b loc color hist)
  (local
    {(: rank : Rank)
     (define rank (match color
                    ['Black 8]
                    ['White 1]))}
    (if (and
         (not (occ? b (Loc 'F rank)))
         (not (occ? b (Loc 'G rank)))
         (not (in-check? (relocate-piece (ChessGame b hist) loc (Loc 'F rank))))
         (not (in-check? (relocate-piece (ChessGame b hist) loc (Loc 'G rank)))))
        (list (Move loc (Loc 'G rank) (Piece 'King color) 'None 'None)
              (Move (Loc 'H rank) (Loc 'F rank)
                    (Piece 'Rook color) 'None 'None))
        '())))

(: castle-left (Board Loc Player (Listof Move) -> (Listof Move)))
;;determines if the king can castle leftwards
(define (castle-left b loc color hist)
  (local
    {(: rank : Rank)
     (define rank (match color
                    ['Black 8]
                    ['White 1]))}
    (if (and
         (not (occ? b (Loc 'D rank)))
         (not (occ? b (Loc 'C rank)))
         (not (occ? b (Loc 'B rank)))
         (not (in-check? (relocate-piece (ChessGame b hist) loc (Loc 'D rank))))
         (not (in-check? (relocate-piece (ChessGame b hist) loc (Loc 'C rank)))))
        (list (Move loc (Loc 'C rank) (Piece 'King color) 'None 'None)
              (Move (Loc 'A rank) (Loc 'D rank)
                    (Piece 'Rook color) 'None 'None))
        '())))
                             

(: moves-along-vectors : PieceType (Listof Vec) Board Loc -> (Listof Move))
;; generalizes queens, bishops, and rooks, which are similar
;; NOTE: this doesn't account for check
(define (moves-along-vectors type vecs b loc)
  (match (board-ref b loc)
    ['None (error "location unoccupied")]
    [(Some (Piece type~ player))
     (if (not (piece-type=? type type~))
         (error (string-append "expected piece type "
                               (symbol->string type)
                               ", got "
                               (symbol->string type~)))
         (map-append (λ ([v : Vec]) (explore b loc v)) vecs))]))

(: moves-rook (Board Loc (Listof Move) -> (Listof Move)))
;; return the list of moves available to a rook
;; NOTE: this doesn't account for check
(define (moves-rook b loc moves)
  (moves-along-vectors 'Rook rook-vecs b loc))

(: moves-bishop (Board Loc (Listof Move) -> (Listof Move)))
;; return the list of moves available to a bishop
;; NOTE: this doesn't account for check
(define (moves-bishop b loc moves)
  (moves-along-vectors 'Bishop bishop-vecs b loc))

(: moves-queen (Board Loc (Listof Move) -> (Listof Move)))
;; return the list of moves available to a queen
;; NOTE: this doesn't account for check
(define (moves-queen b loc moves)
  (moves-along-vectors 'Queen queen-vecs b loc))

;; === checking for check

(: threatened-along-vectors? : PieceType (Listof Vec) Board Loc -> Boolean)
;; generalizes bishop, rook, queen
(define (threatened-along-vectors? type vecs b loc)
  (match (board-ref b loc)
    ['None (error "empty square")]
    [(Some (Piece _ p))
     (local
       {(define enemy (Piece type (opponent p)))
        (define (enemy? [vec : Vec])
          (piece-along? b loc enemy vec))}
       (ormap enemy? vecs))]))

(: bishop-threatens? : Board Loc -> Boolean)
;; does an enemy bishop threaten the piece at the location?
(define (bishop-threatens? b loc)
  (threatened-along-vectors? 'Bishop bishop-vecs b loc))

(: rook-threatens? : Board Loc -> Boolean)
;; does an enemy rook threaten the piece at the location?
(define (rook-threatens? b loc)
  (threatened-along-vectors? 'Rook rook-vecs b loc))

(: queen-threatens? : Board Loc -> Boolean)
;; does an enemy queen threaten the piece at the location?
(define (queen-threatens? b loc)
  (threatened-along-vectors? 'Queen queen-vecs b loc))

(: pawn-threatens? : Board Loc -> Boolean)
;; does an enemy pawn threaten the piece at the location?
(define (pawn-threatens? b loc)
  (match (board-ref b loc)
    ['None (error "empty square")]
    [(Some (Piece _ p))
     (local
       {(define delta-rank (match p ['Black -1] ['White 1]))
        (define opp-pawn (Piece 'Pawn (opponent p)))}
       (or (piece-at? b opp-pawn (add-loc loc delta-rank -1))
           (piece-at? b opp-pawn (add-loc loc delta-rank 1))))]))

(: knight-threatens? : Board Loc -> Boolean)
;; does an enemy knight threaten the piece at the location?
(define (knight-threatens? b loc)
  (match (board-ref b loc)
    ['None (error "empty square")]
    [(Some (Piece _ p))
     (local
       {(define k (Piece 'Knight (opponent p)))
        (define (enemy-knight? [vec : Vec])
          (match vec
            [(Vec delta-rank delta-file)
             (piece-at? b k (add-loc loc delta-rank delta-file))]))}
       (ormap enemy-knight? knight-vecs))]))

(: king-location : Board Player -> Loc)
;; return the location of the given king
(define (king-location board color)
  (local
    {(: k? : Square -> Boolean)
     (define (k? sq)
       (match sq
         ['None #f]
         [(Some piece) (piece=? piece (Piece 'King color))]))}
    (int->loc (val-of (index-of k? board)))))

(: whose-turn : ChessGame -> Player)
;; determine who moves next in the given game
;; NOTE: most recent move is first in the history
(define (whose-turn g)
  (match (ChessGame-history g)
    [(cons (Move _ _ (Piece _ 'White) _ _) _) 'Black]
    [_ 'White]))

(: in-check? : ChessGame -> Boolean)
;; Is the current player in check?
(define (in-check? game)
  (match game
    [(ChessGame board _)
     (local {(define king (king-location board (whose-turn game)))}
       (ormap (λ ([test : (Board Loc -> Boolean)]) (test board king))
              (list pawn-threatens?
                    bishop-threatens?
                    rook-threatens?
                    queen-threatens?
                    knight-threatens?)))]))

;; === available moves and legality

(: mover : PieceType -> (Board Loc (Listof Move) -> (Listof Move)))
;; match a piece type to a "mover" function
(define (mover type)
  (match type
    ['Pawn   moves-pawn]
    ['Knight moves-knight]
    ['Bishop moves-bishop]
    ['Rook   moves-rook]
    ['Queen  moves-queen]
    ['King   moves-king]))

(: moves-piece : ChessGame Loc -> (Listof Move))
;; return the moves a piece can make
;; note: it must be that player's turn for any moves to be available
(define (moves-piece game loc)
  (match game
    [(ChessGame board moves)
     (match (board-ref board loc)
       ['None '()]
       [(Some (Piece type p~))
        (if (not (player=? (whose-turn game) p~))
            '()
            (local
              {(: legal? (Move -> Boolean))
               (define (legal? m)
                 (match m
                   [(Move src dst _ _ _)
                    (not (in-check? (relocate-piece game src dst)))]))}
              (filter legal? ((mover type) board loc moves))))])]))

(check-expect (moves-piece (ChessGame (strings->board (list "--K-----"
                                                            "--------"
                                                            "------N-"
                                                            "----Pp--"
                                                            "--------"
                                                            "--------"
                                                            "--------"
                                                            "---k----"))
                                      (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'Black)
                                                        'None 'None)))
                           (Loc 'F 5))
              (list
               (Move (Loc 'F 5) (Loc 'F 6) (Piece 'Pawn 'White) 'None 'None)
               (Move (Loc 'F 5) (Loc 'E 6) (Piece 'Pawn 'White)
                     (Some (Piece 'Pawn 'Black)) 'None)
               (Move (Loc 'F 5) (Loc 'G 6) (Piece 'Pawn 'White)
                     (Some (Piece 'Knight 'Black)) 'None)))

(check-expect (moves-piece (ChessGame (strings->board (list "--K-----"
                                              "-----Q--"
                                              "------N-"
                                              "----Pp--"
                                              "--------"
                                              "--------"
                                              "--------"
                                              "-----k--"))
                                      (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'Black)
                                                        'None 'None)))
                           (Loc 'F 5))
              (list (Move (Loc 'F 5) (Loc 'F 6) (Piece 'Pawn 'White) 'None 'None)))

(check-expect (moves-piece (ChessGame (strings->board (list "--K-----"
                                                            "-----Q--"
                                                            "------N-"
                                                            "----Pp--"
                                                            "--------"
                                                            "--------"
                                                            "---p-p--"
                                                            "r---k--r"))
                                      (list (Move (Loc 'E 7) (Loc 'E 5)
                                                        (Piece 'Pawn 'Black)
                                                        'None 'None)
                                            (Move (Loc 'H 2) (Loc 'H 1)
                                                  (Piece 'Rook 'White)
                                                  'None 'None)))
                                      (Loc 'E 1))
                           (list
 (Move (Loc 'E 1) (Loc 'C 1) (Piece 'King 'White) 'None 'None)
 (Move (Loc 'A 1) (Loc 'D 1) (Piece 'Rook 'White) 'None 'None)
 (Move (Loc 'E 1) (Loc 'E 2) (Piece 'King 'White) 'None 'None)
 (Move (Loc 'E 1) (Loc 'F 1) (Piece 'King 'White) 'None 'None)
 (Move (Loc 'E 1) (Loc 'D 1) (Piece 'King 'White) 'None 'None)))

                           
              

(: legal-move? : ChessGame Move -> Boolean)
;; a Move is legal if
;; - the named piece is actually at the source location,
;; - the named piece is capable of moving to the dest location per chess rules,
;; - the captured piece is actually at the dest location, and
;; - the move does not expose the moving player to check
;; TODO: castling
;; TODO: en passant
;; TODO: promotion
(define (legal-move? g m)
  (match g
    [(ChessGame b hist) (match m
                          [(Move src dst piece cap 'None)
                           (and (piece-at? b piece (Some src))
                                (ormap (λ ([m~ : Move]) (move=? m m~))
                                       (moves-piece g src))
                                (match cap
                                  ['None (unocc? b dst)]
                                  [(Some c)
                                   (match hist
                                     ['() (piece-at? b c (Some dst))]
                                     [(cons first rest)
                                      (if
                                       (en-passant? m first)
                                       (match (Piece-color piece)
                                         ['White (piece-at?
                                                  b c
                                                  (Some (Loc (Loc-file dst)
                                                             (val-of (add1-rank (Loc-rank dst))))))]
                                         ['Black (piece-at?
                                                  b c
                                                  (Some (Loc (Loc-file dst)
                                                             (val-of (sub1-rank (Loc-rank dst))))))])
                                                         (piece-at? b c (Some dst)))])])
                        (not (in-check? (relocate-piece g src dst))))]
[_ (error "todo")])]))


#|(check-expect (legal-move? (ChessGame
  (list
   (Some (Piece 'Rook 'White))
   (Some (Piece 'Knight 'White))
   (Some (Piece 'Bishop 'White))
   (Some (Piece 'Queen 'White))
   (Some (Piece 'King 'White))
   (Some (Piece 'Bishop 'White))
   (Some (Piece 'Knight 'White))
   (Some (Piece 'Rook 'White))
   (Some (Piece 'Pawn 'White))
   'None
   (Some (Piece 'Pawn 'White))
   'None
   (Some (Piece 'Pawn 'White))
   'None
   (Some (Piece 'Pawn 'White))
   (Some (Piece 'Pawn 'White))
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   (Some (Piece 'Pawn 'White))
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Pawn 'White))
   'None
   (Some (Piece 'Pawn 'White))
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   'None
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Pawn 'Black))
   'None
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Pawn 'Black))
   (Some (Piece 'Rook 'Black))
   (Some (Piece 'Knight 'Black))
   (Some (Piece 'Bishop 'Black))
   (Some (Piece 'Queen 'Black))
   (Some (Piece 'King 'Black))
   (Some (Piece 'Bishop 'Black))
   (Some (Piece 'Knight 'Black))
   (Some (Piece 'Rook 'Black)))
  (list
   (Move (Loc 'D 2) (Loc 'D 4) (Piece 'Pawn 'White) 'None 'None)
   (Move (Loc 'C 5) (Loc 'C 4) (Piece 'Pawn 'Black) 'None 'None)
   (Move (Loc 'B 2) (Loc 'B 4) (Piece 'Pawn 'White) 'None 'None)
   (Move (Loc 'C 7) (Loc 'C 5) (Piece 'Pawn 'Black) 'None 'None)
   (Move (Loc 'F 2) (Loc 'F 4) (Piece 'Pawn 'White) 'None 'None)))
             (Move (Loc 'C 4) (Loc 'D 3) (Piece 'Pawn 'Black)
                                 (Some (Piece 'Pawn 'White)) 'None)) #t)
|#
(: relocate-piece : ChessGame Loc Loc -> ChessGame)
;; uncritically relocate piece without updating history
;; - this is useful for exploring hypotheticals
(define (relocate-piece g src dst)
  (match g
    [(ChessGame b hist)
     (match (board-ref b src)
       ['None g]
       [(Some pc)
        (local
          {(define b~ (board-update (board-update b src 'None) dst (Some pc)))}
        (ChessGame b~ hist))])]))

(: apply-move : ChessGame Move -> ChessGame)
;; apply move if legal
;;TODO promotion
(define (apply-move g m)
  (if (not (legal-move? g m))
      g
      (match g
        [(ChessGame b hist)
         (match m
           [(Move src dst pc _ 'None)
            (match hist
              ['() (ChessGame (board-update (board-update b src 'None) dst (Some pc))
                       (cons m hist))]
              [(cons first rest)
               (cond
              [(castling? m) (apply-castle g m)]
              [(en-passant? m first) (apply-passant g m)]
              
             [else (ChessGame (board-update (board-update b src 'None) dst (Some pc))
                       (cons m hist))])])]
           ;;[(Move srcp dstp pcp _ (Some promp)) (apply-promotion g m promp)]
           [_ (error "promotion")]
           )])))

(check-expect (apply-move new-game (Move (Loc 'D 2) (Loc 'D 3) (Piece 'Pawn 'White) 'None 'None))
              (ChessGame (strings->board (list "RNBQKBNR"
                                               "PPPPPPPP"
                                               "--------"
                                               "--------"
                                               "--------"
                                               "---p----"
                                               "ppp-pppp"
                                               "rnbqkbnr"))
                         (list (Move (Loc 'D 2) (Loc 'D 3) (Piece 'Pawn 'White) 'None 'None))))
               
          
            
(: of-color? (Player Piece -> Boolean))
(define (of-color? color piece)
  (player=? (Piece-color piece) color))


(: en-passant? : Move Move -> Boolean)
;;checks if move is en passant
(define (en-passant? m old)
     (match m
       [(Move src dst (Piece 'Pawn p) _ 'None)
              (match old
                [(Move olds oldd (Piece 'Pawn oldp) _ _)
                 (and (= (Loc-rank src) (Loc-rank oldd))
                      (symbol=? (Loc-file dst) (Loc-file oldd))
                      (= 2 (abs (- (Loc-rank olds) (Loc-rank oldd)))))]
                [_ #f])]
       [_ #f]))

(check-expect (en-passant? (Move (Loc 'C 4) (Loc 'D 3) (Piece 'Pawn 'Black)
                                 (Some (Piece 'Pawn 'White)) 'None)
                           (Move (Loc 'D 2) (Loc 'D 4) (Piece 'Pawn 'White) 'None 'None)) #t)

(check-expect (en-passant? (Move (Loc 'C 4) (Loc 'D 3) (Piece 'King 'Black)
                                 (Some (Piece 'Pawn 'White)) 'None)
                           (Move (Loc 'D 2) (Loc 'D 4) (Piece 'Pawn 'White) 'None 'None)) #f)

(check-expect (en-passant? (Move (Loc 'C 4) (Loc 'D 3) (Piece 'Pawn 'Black)
                                 (Some (Piece 'Pawn 'White)) 'None)
                           (Move (Loc 'D 2) (Loc 'D 4) (Piece 'King 'White) 'None 'None)) #f)

(check-expect (en-passant? (Move (Loc 'C 4) (Loc 'D 3) (Piece 'Pawn 'Black)
                                 (Some (Piece 'Pawn 'White)) 'None)
                           (Move (Loc 'D 2) (Loc 'D 3) (Piece 'Pawn 'White) 'None 'None)) #f)

(: apply-passant : ChessGame Move -> ChessGame)
;;applies en passant move
(define (apply-passant g m)
  (match g
    [(ChessGame b hist) (match m
                          [(Move src (Loc dfile drank) (Piece 'Pawn color) _ _)
                           (ChessGame (board-update
                                       (board-update
                                        (board-update b src 'None)
                                        (Loc dfile drank) (Some (Piece 'Pawn color)))
                                       (if (symbol=? color 'White)
                                           (Loc dfile (val-of (sub1-rank drank)))
                                           (Loc dfile (val-of (add1-rank drank))))
                                       'None) hist)]
                          [_ (error "not en passant")])]))
    

(: castling? : Move -> Boolean)
;;checks if move is a castle
(define (castling? move)
  (match move
    [(Move src dst (Piece 'King player) 'None 'None)
     (= (abs (- (file->int (Loc-file src)) (file->int (Loc-file dst)))) 2)]
    [_ #f]))
     
(: apply-castle : ChessGame Move -> ChessGame)
;;applies a castling move
(define (apply-castle g move)
  (match move
    [(Move src dst p _ _)
     (match dst
       [(Loc 'C 1) (ChessGame (board-update
                               (board-update
                                (board-update
                                 (board-update (ChessGame-board g) src 'None)
                                 dst (Some (Piece 'King 'White)))
                                (Loc 'A 1) 'None)
                               (Loc 'D 1) (Some (Piece 'Rook 'White)))
                       (append (list (Move (Loc 'A 1) (Loc 'D 1)
                                           (Piece 'Rook 'White) 'None 'None) move)
                               (ChessGame-history g)))]
       [(Loc 'G 1) (ChessGame (board-update
                               (board-update
                                (board-update
                                 (board-update (ChessGame-board g) src 'None)
                                 dst (Some (Piece 'King 'White)))
                                (Loc 'H 1) 'None)
                               (Loc 'F 1) (Some (Piece 'Rook 'White)))
                       (append (list (Move (Loc 'H 1) (Loc 'F 1)
                                           (Piece 'Rook 'White) 'None 'None) move)
                               (ChessGame-history g)))]
       [(Loc 'C 8) (ChessGame (board-update
                               (board-update
                                (board-update
                                 (board-update (ChessGame-board g) src 'None)
                                 dst (Some (Piece 'King 'Black)))
                                (Loc 'A 8) 'None)
                               (Loc 'D 8) (Some (Piece 'Rook 'Black)))
                       (append (list (Move (Loc 'A 8) (Loc 'D 8)
                                           (Piece 'Rook 'Black) 'None 'None) move)
                               (ChessGame-history g)))]
       [(Loc 'G 8) (ChessGame (board-update
                               (board-update
                                (board-update
                                 (board-update (ChessGame-board g) src 'None)
                                 dst (Some (Piece 'King 'Black)))
                                (Loc 'H 8) 'None)
                               (Loc 'F 8) (Some (Piece 'Rook 'Black)))
                       (append (list (Move (Loc 'H 8) (Loc 'F 8)
                                           (Piece 'Rook 'Black) 'None 'None) move)
                               (ChessGame-history g)))]
       [_ (error "not a castling move")])]))
    
(: moves-player : ChessGame -> (Listof Move))
;; collect all the available moves for the current player
(define (moves-player game)
  (match game
    [(ChessGame board history)
     (local
       {(define current-color (whose-turn game))
        (: moves : Loc -> (Listof Move))
        (define (moves loc)
          (match (board-ref board loc)
            [(Some piece)
             (if (of-color? current-color piece)
                 (moves-piece game loc)
                 '())]
            ['None '()]))}
       (map-append moves every-loc))]))

;; === end game conditions

(: checkmate? : ChessGame -> Boolean)
(define (checkmate? g)
  (and (in-check? g)
       (empty? (moves-player g))))

(check-expect (checkmate? new-game) #f)

(: stalemate? (ChessGame -> Boolean))
(define (stalemate? g)
  (and (not (in-check? g))
       (empty? (moves-player g))))

(check-expect (stalemate? new-game) #f)

(test)