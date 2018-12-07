using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using MaterialDesignThemes.Wpf;

namespace Core
{
    /// <summary>
    /// Interaction logic for InputDialog.xaml
    /// </summary>
    public partial class InputDialog : UserControl
    {
        public InputDialog(string title, string hint1, string hint2 = null)
        {
            InitializeComponent();
            Loaded += (s, e) =>
            {
                this.Title.Content = title;
                HintAssist.SetHint(this.TextBox1, hint1);
                if (hint2 == null)
                {
                    return;
                }
                HintAssist.SetHint(this.TextBox2, hint2);
                this.TextBox2.Visibility = Visibility.Visible;
            };
        }
        
    }

    public class NotEmptyValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, CultureInfo cultureInfo)
        {
            return string.IsNullOrWhiteSpace((value ?? "").ToString())
                ? new ValidationResult(false, "Field is required.")
                : ValidationResult.ValidResult;
        }
    }
}
