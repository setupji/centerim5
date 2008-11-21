/* gtktexttag.c - text tag object
 *
 * Copyright (c) 1992-1994 The Regents of the University of California.
 * Copyright (c) 1994-1997 Sun Microsystems, Inc.
 * Copyright (c) 2000      Red Hat, Inc.
 * Tk -> Gtk port by Havoc Pennington <hp@redhat.com>
 * Copyright (c) 2008      Mark Pustjens <pustjens@dds.nl>
 * Gtk -> C++ port
 *
 * This software is copyrighted by the Regents of the University of
 * California, Sun Microsystems, Inc., and other parties.  The
 * following terms apply to all files associated with the software
 * unless explicitly disclaimed in individual files.
 *
 * The authors hereby grant permission to use, copy, modify,
 * distribute, and license this software and its documentation for any
 * purpose, provided that existing copyright notices are retained in
 * all copies and that this notice is included verbatim in any
 * distributions. No written agreement, license, or royalty fee is
 * required for any of the authorized uses.  Modifications to this
 * software may be copyrighted by their authors and need not follow
 * the licensing terms described here, provided that the new terms are
 * clearly indicated on the first page of each file where they apply.
 *
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION,
 * OR ANY DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 * NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS,
 * AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
 * MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * GOVERNMENT USE: If you are acquiring this software on behalf of the
 * U.S. government, the Government shall have only "Restricted Rights"
 * in the software and related documentation as defined in the Federal
 * Acquisition Regulations (FARs) in Clause 52.227.19 (c) (2).  If you
 * are acquiring the software on behalf of the Department of Defense,
 * and the Government shall have only "Restricted Rights" as defined
 * in Clause 252.227-7013 (c) (1) of DFARs.  Notwithstanding the
 * foregoing, the authors grant the U.S. Government and others acting
 * in its behalf permission to use and distribute the software in
 * accordance with the terms specified in this license.
 *
 */

//#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
//#error "Only <gtk/gtk.h> can be included directly."
//#endif

#ifndef __TEXT_TAG_H__
#define __TEXT_TAG_H__

//#include <gdk/gdk.h>
//#include <gtk/gtkenums.h>

#include "TextTagTable.h"

#include <glib.h>

/* Text wrap */
typedef enum
{
  WRAP_NONE,
  WRAP_CHAR,
  WRAP_WORD,
  WRAP_WORD_CHAR
} WrapMode;

/* Not needed, retained for compatibility -Yosh */
//#include <gtk/gtkobject.h>

/*typedef struct _GtkTextIter GtkTextIter;
typedef struct _GtkTextTagTable GtkTextTagTable;

typedef struct _GtkTextAttributes GtkTextAttributes;*/

typedef struct {
  gint high;
  gint low;
  gint delta;
} DeltaData;


#define GTK_TYPE_TEXT_TAG            (gtk_text_tag_get_type ())
#define GTK_TEXT_TAG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TEXT_TAG, GtkTextTag))
#define GTK_TEXT_TAG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEXT_TAG, GtkTextTagClass))
#define GTK_IS_TEXT_TAG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TEXT_TAG))
#define GTK_IS_TEXT_TAG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEXT_TAG))
#define GTK_TEXT_TAG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEXT_TAG, GtkTextTagClass))

#define GTK_TYPE_TEXT_ATTRIBUTES     (gtk_text_attributes_get_type ())

//typedef struct _GtkTextTag GtkTextTag;
//typedef struct _GtkTextTagClass GtkTextTagClass;

class TextTagTable;
class TextAttributes;

//TODO this is temporary until cppconsui has a notion of colors
//  note that `pixel' is basically what we can map to a ncurses color number
typedef struct {
  guint32 pixel;
  guint16 red;
  guint16 green;
  guint16 blue;
} Color; //GdkColor;

typedef int Colormap; //TODO same as for Color

class TextTag
{
	public:
		TextTag(const gchar *name);
		~TextTag(void);

		TextTagTable *table; //TODO make private and make getter/setter?


		char *name; /* Name of this tag.  This field is actually
			     * a pointer to the key from the entry in
			     * tkxt->tagTable, so it needn't be freed
			     * explicitly. */
		int priority; /* Priority of this tag within widget.  0
                         * means lowest priority.  Exactly one tag
                         * has each integer value between 0 and
                         * numTags-1. */

		//typedef struct _GtkTextBTreeNode GtkTextBTreeNode;

/* values should already have desired defaults; this function will override
 * the defaults with settings in the given tags, which should be sorted in
 * ascending order of priority
*/
static void array_sort(TextTag **tag_array_p, guint len);

/* ensure colors are allocated, etc. for drawing */
/*
void                _gtk_text_attributes_realize   (GtkTextAttributes *values,
                                                    GdkColormap       *cmap,
                                                    GdkVisual         *visual);
*/
/* free the stuff again */
/*void                _gtk_text_attributes_unrealize (GtkTextAttributes *values,
                                                    GdkColormap       *cmap,
                                                    GdkVisual         *visual);
*/

bool affects_size               (void);
bool affects_nonsize_appearance (void);


  /*
   * Information for displaying text with this tag.  The information
   * belows acts as an override on information specified by lower-priority
   * tags.  If no value is specified, then the next-lower-priority tag
   * on the text determins the value.  The text widget itself provides
   * defaults if no tag specifies an override.
   */

  TextAttributes *values;
  
  /* Flags for whether a given value is set; if a value is unset, then
   * this tag does not affect it.
   */
  guint bg_color_set : 1;
  guint bg_stipple_set : 1;
  guint fg_color_set : 1;
  guint scale_set : 1;
  guint fg_stipple_set : 1;
  guint justification_set : 1;
  guint left_margin_set : 1;
  guint indent_set : 1;
  guint rise_set : 1;
  guint strikethrough_set : 1;
  guint right_margin_set : 1;
  guint pixels_above_lines_set : 1;
  guint pixels_below_lines_set : 1;
  guint pixels_inside_wrap_set : 1;
  guint tabs_set : 1;
  guint underline_set : 1;
  guint wrap_mode_set : 1;
  guint bg_full_height_set : 1;
  guint invisible_set : 1;
  guint editable_set : 1;
  guint language_set : 1;
  guint pg_bg_color_set : 1;

  /* Whether these margins accumulate or override */
  guint accumulative_margin : 1;

  guint pad1 : 1;

  /* TODO make event of this
  bool (* event) (GtkTextTag        *tag,
                      GObject           *event_object, * widget, canvas item, whatever *
                      GdkEvent          *event,        * the event itself *
                      const GtkTextIter *iter);        * location of event in buffer *
  */

  /* values should already have desired defaults; this function will override
 * the defaults with settings in the given tags, which should be sorted in
 * ascending order of priority
*/
bool affects_size               (TextTag *tag);
bool affects_nonsize_appearance (TextTag *tag);

void         set_priority ( gint               priority);

	protected:

	private:

gint         GetPriority (void);
void         SetPriority (gint priority);
/*TODO something with an event?
bool     gtk_text_tag_event        (GtkTextTag        *tag,
                                        GObject           *event_object,
                                        GdkEvent          *event,
                                        const GtkTextIter *iter);
*/


	void set_bg_color (Color *color);
	void set_fg_color (Color *color);
	void set_pg_bg_color (Color *color);

	static void delta_priority_foreach(TextTag *tag, DeltaData *user_data);
	gint get_priority(void);
	static bool tag_sort_func (TextTag *first, TextTag *second);


};

/*
 * Style object created by folding a set of tags together
 */

class TextAppearance
{
	public:
  /*< public >*/
  Color bg_color;
  Color fg_color;
  //GdkBitmap *bg_stipple;
  //GdkBitmap *fg_stipple;

  /* super/subscript rise, can be negative */
  //gint rise;

  /*< public >*/
  guint underline : 1;          /* PangoUnderline */
  //guint strikethrough : 1;
	protected:

	private:
  /*< private >*/
  /* I'm not sure this can really be used without breaking some things
   * an app might do :-/
   */
  //gpointer padding1;


  /* Whether to use background-related values; this is irrelevant for
   * the values struct when in a tag, but is used for the composite
   * values struct; it's true if any of the tags being composited
   * had background stuff set.
   */
  //guint draw_bg : 1;
  
  /* These are only used when we are actually laying out and rendering
   * a paragraph; not when a GtkTextAppearance is part of a
   * GtkTextAttributes.
   */
  guint inside_selection : 1;
  guint is_text : 1;

  /*< private >*/
  guint pad1 : 1;
  guint pad2 : 1;
  guint pad3 : 1;
  guint pad4 : 1;
};

class TextAttributes
{
	public:
		TextAttributes(void);
		TextAttributes(Colormap *cmap);
		void unrealize (Colormap *cmap);
		void fill_from_tags (TextTag **tags, guint n_tags);
		static void copy_values (TextAttributes *src, TextAttributes *dest);

  /*< private >*/
  guint refcount;

  /*< public >*/
  TextAppearance appearance;

  //GtkJustification justification;
  //GtkTextDirection direction;

  /* Individual chunks of this can be set/unset as a group */
  //PangoFontDescription *font;

  gdouble font_scale;
  
  gint left_margin;

  gint indent;  

  gint right_margin;

  gint pixels_above_lines;

  gint pixels_below_lines;

  gint pixels_inside_wrap;

  //PangoTabArray *tabs;

  WrapMode wrap_mode;        /* How to handle wrap-around for this tag.
                                 * Must be GTK_WRAPMODE_CHAR,
                                 * GTK_WRAPMODE_NONE, GTK_WRAPMODE_WORD
                                 */

  //PangoLanguage *language;

  /*< private >*/
  Color *pg_bg_color;

  /*< public >*/
  /* hide the text  */
  guint invisible : 1;

  /* Background is fit to full line height rather than
   * baseline +/- ascent/descent (font height)
   */
  guint bg_full_height : 1;

  /* can edit this text */
  guint editable : 1;

  /* colors are allocated etc. */
  guint realized : 1;

  /*< private >*/
  guint pad1 : 1;
  guint pad2 : 1;
  guint pad3 : 1;
  guint pad4 : 1;

	void unref(void);
	TextAttributes *ref         (void);

	protected:
	private:
		//TODO copy operator and such


};

#endif /* __TEXT_TAG_H__ */
