#ifndef QTCONFIGINC_
#define QTCONFIGINC_

#include <QSettings>
#include <string>
#include <vector>
#include <map>

/// Provide an interface for persistent configuration
/// management. Configuration items are specified
/// as key:value pairs.
/// Calls which fetch a configuration value 
/// also specify a default value for that key.
/// The configuration is written with each fetch,
/// so that a configuration will contain the default values
/// if they haven't been set.
/// Configurations are always synced when a value is written.
class QtConfig : protected QSettings {
    public:
    /// Constructor. The organization and application keys provide for
    /// categorization of the configuration set, as provided by the QSettings
    /// scheme.
    /// @param organization The organization.
    /// @param application The application.
    QtConfig(
            const std::string organization,
                const std::string application);

    /// Constructor which specifies theconfig file path.
    /// @param organization The organization.
    /// @param application The application.
    QtConfig(const std::string filePath);

        /// Destructor
        virtual ~QtConfig();

        /// Set the value of the configuration item.
        /// @param key Configuration item name.
        /// @param t Configuration item value.
        void setString(
                std::string key,
                    std::string t);
        
        /// Fetch the value of the configuration item.
        /// @param key The configuration item name.
        /// @param defaultValue The default value assigned to this configuration item.
        /// @returns The configuration item value. If the configuration item has 
        /// not yet been defined, it is created with the default value,
        /// and the default value is returned.
        std::string getString(
                std::string key,
                    std::string defaultValue);

        /// Set the value of the configuration item.
        /// @param key Configuration item name.
        /// @param t Configuration item value.
        void setDouble(
                std::string key,
                    double d);
        
        /// Fetch the value of the configuration item.
        /// @param key The configuration item name.
        /// @param defaultValue The default value assigned to this configuration item.
        /// @returns The configuration item value. If the configuration item has 
        /// not yet been defined, it is created with the default value,
        /// and the default value is returned.
        double getDouble(
                std::string key,
                    double defaultValue);

        /// Set the value of the configuration item.
        /// @param key Configuration item name.
        /// @param f Configuration item value.
        void setFloat(
                std::string key,
                    float f);
        
        /// Fetch the value of the configuration item.
        /// @param key The configuration item name.
        /// @param defaultValue The default value assigned to this configuration item.
        /// @returns The configuration item value. If the configuration item has 
        /// not yet been defined, it is created with the default value,
        /// and the default value is returned.
        float getFloat(
                std::string key,
                    float defaultValue);

        /// Set the value of the configuration item.
        /// @param key Configuration item name.
        /// @param i Configuration item value.
        void setInt(
                std::string key,
                    int i);
        
        /// Fetch the value of the configuration item.
        /// @param key The configuration item name.
        /// @param defaultValue The default value assigned to this configuration item.
        /// @returns The configuration item value. If the configuration item has 
        /// not yet been defined, it is created with the default value,
        /// and the default value is returned.
        int getInt(
                std::string key,
                    int defaultValue);

        /// Set the value of the configuration item.
        /// @param key Configuration item name.
        /// @param b Configuration item value.
        void setBool(
                std::string key,
                    bool b);
        
        /// Fetch the value of the configuration item.
        /// @param key The configuration item name.
        /// @param defaultValue The default value assigned to this configuration item.
        /// @returns The configuration item value. If the configuration item has 
        /// not yet been defined, it is created with the default value,
        /// and the default value is returned.
        bool getBool(
                std::string key,
                    bool defaultValue);

        /// Set the values of an array type configuration item.
        /// @param key Configuration item name.
        /// @param subKey The field name within the array. 
        /// This allows arrays to contain multiple field keys.
        /// @param values Vector of configuration item values.
        void setArray(
                std::string key,
                    std::string subKey,
                    std::vector<std::vector<int> > values);

        /// Get the values of an array type configuration item.
        /// @param key Configuration item name.
        /// @param subKey The field name within the array. 
        /// This allows arrays to contain multiple field keys.
        /// @param values Vector of default configuration item values.
        std::vector<std::vector<int> > getArray(
                std::string key,
                    std::string subKey,
                    std::vector<std::vector<int> > defaultValues);

        /// Set the values of an array type configuration item.
        /// @param key Configuration array group name.
        /// This allows arrays to contain multiple field keys.
        /// @param values es Vector of default configuration item values. Each
        /// map in the vector contains a dictionary of key/value pairs for
        /// one array entry. The keys do not have to be the same for each
        /// array entry, although that could get confusing.
        void setArray(
                std::string key,
                std::vector<std::map<std::string, std::string> > values);

        /// Get the values of an array type configuration item.
        /// @param key Configuration array group name.
        /// @param defaultValues Vector of default configuration item values. Each
        /// map in the vector contains a dictionary of key/value pairs for
        /// one array entry. The keys do not have to be the same for each
        /// array entry, although that could get confusing.
        std::vector<std::map<std::string, std::string> > getArray(
                std::string key,
                std::vector<std::map<std::string, std::string> > defaultValues);

        /// Get the configuration groups below the given group.
        /// @param topGroup Limit list to keys below this group.
        /// @returns A list of the groups below this key.
        std::vector<std::string> childGroups(
                std::string topGroup);

        /// @returns The name of the underlying configuration file.
        std::string fileName();

        /// @return All of the keys in the configuration.
        std::vector<std::string> getKeys();
};

#endif
