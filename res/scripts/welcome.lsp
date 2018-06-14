; Nyquist Coder Welcome Tune

; Define function
; Envelope will cause sound to fade in and out by 0.05s.

(defun
    create-envelope (note-length)
    (pwl 0.05 1 (- note-length 0.05) 1 note-length)
)

; Setting params

(setq note-length-short 0.5)
(setq note-length-long 0.8)

(setq envelope-short (create-envelope note-length-short))
(setq envelope-long (create-envelope note-length-long))

; Creating notes

(setq note1 (mult (osc 48 note-length-long) envelope-long))
(setq note2 (mult (osc 43 note-length-short) envelope-short))
(setq note3 (mult (osc 51 note-length-short) envelope-short))

; Composing tune

(setq tune
(seq 
    (cue note1)
    (cue note2)
    (cue note3)
))

; Enjoy the Welcome Tune!

(play tune)

; EOF