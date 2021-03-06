\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ml}
\LoadClass[10pt, a4paper, oneside]{article}

\usepackage[utf8x]{inputenc}
\usepackage[english]{babel}
\usepackage[T1]{fontenc}
\usepackage[usenames,dvipsnames]{xcolor}
\definecolor{darkblue}{RGB}{0,0,85}
\usepackage[colorlinks=true, linkcolor=darkblue,citecolor=darkblue,raiselinks=true,urlcolor=darkblue]{hyperref}
\usepackage{graphicx}
\usepackage{palatino}
\usepackage{a4}
\usepackage{url}
\usepackage{fancyhdr}
\usepackage{wrapfig}
\usepackage{lipsum}
\usepackage{marvosym}
\usepackage{natbib}

\widowpenalty=1000
\clubpenalty=1000


% -----------------------------------------------------------------------
% Page layout

% oberen Seitenrand reduzieren
\addtolength{\voffset}{-1cm}

% Kein Erstzeileneinzug beim Absatzanfang
% Sieht aber nur gut aus, wenn man zwischen Absätzen viel Platz einbaut
\setlength{\parindent}{1ex}

% Abstand zwischen zwei Absätzen
\setlength{\parskip}{1ex}

% Seitenränder verändern
\addtolength{\evensidemargin}{-1cm}
\addtolength{\oddsidemargin}{-1cm}

% Textbreite erhöhen
\textwidth = 440pt

\newcommand{\thissemester}{Session 1,~2013}
\newcommand{\thistitle}{COMP9417 - Machine Learning \& Data Mining}

% -----------------------------------------------------------------------
% Main macros
\renewcommand{\maketitle}[6]
          {\@maketitle{#1}{#2}{#3}{#4}{#5}{#6}{Project Report}}
\renewcommand\thesection{\arabic{section}}
\renewcommand\thesubsection{\arabic{section}.\arabic{subsection}}

\newcommand\skipsection{\stepcounter{section}}

% -----------------------------------------------------------------------
% maketitle
% param1: Project#
% param2: Due date
% param3: Name1
% param4: Student number1
% param5: Name2
% param6: Student number2
% param7: "Project Report"
\renewcommand{\@maketitle}[7]
{
    \hspace*{-.8em}\begin{tabular}[t]{lr}
        \thistitle 		& {#3}, {#4}\\
        \thissemester	& {#5}, {#6}\\
        				& 			\\
        {\Huge\sf {#7} {#1} } & Due: {#2} \\
      \end{tabular}\\
    \rule{\linewidth}{1pt}

% -----------------------------------------------------------------------
% Create headings (Thanks to Elmar Loos)

    \fancyhead{} %clear all fields
    \fancyhead[LO,RE]{ML \& DM,~\thissemester}
    \fancyhead[RO,LE]{Name: {#3}, {#5}}
    \fancyfoot{} %clear all fields
    \fancyfoot[RO,LE]{\thepage}
}

% Standardmäßig das Fancy-Layout auswählen
% Seitenweise kann es z.B. mit "\thispagestyle{empty}" ausgeschaltet werden
\pagestyle{fancy}
\thispagestyle{empty}

\endinput
